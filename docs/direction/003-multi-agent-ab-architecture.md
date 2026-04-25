# Multi-agent concurrent matching + robust A/B benchmarking

**Status:** plan, not yet implemented. Begun 2026-04-25 from a single-agent
single-backend orchestrator with ad-hoc backend A/B (random per-session
backend pick).

## Goals

1. **Concurrent agents** — N workers running matching sessions in parallel,
   any mix of backends and models. Default config: 1 claude + 1 codex.
2. **Race-free DB and src-file mutations** — concurrent agents never lose
   each other's writes, never double-claim a function, never produce
   half-written commits.
3. **Statistically defensible A/B benchmarks** — comparing
   `(backend, model)` tuples. Not just claude vs codex; opus 4.6 vs 4.7 is
   the same comparison shape.
4. **Per-attempt audit trail** — for any function, we can answer:
   *did backend X attempt this? when? what happened?* Without log mining.
5. **TUI that shows N concurrent sessions clearly**, with backend×model
   identity visually distinct.

The goal is NOT to reach "match every function as fast as possible" — at
the cost of poisoned training data or unreproducible benchmarks. Quality
of the matched corpus and quality of the comparison data are first-class.

## Current state

Single-threaded orchestrator (`tools/orchestrator.py`). One agent at a
time, sequential session loop. Backend A/B added 2026-04-24 picks
(claude or codex) randomly per session but races head-to-head only by
chance — and from the existing pool, not a controlled split.

### Concurrency-unsafe assumptions in the current code

Mapped in detail by an exploration agent against `tools/orchestrator.py`.
Not exhaustive but representative:

- **Batch picking is not atomic with status mutation.** `pick_next_batch`
  → `set_batch_status("in_progress")` → `save_db` is three separate
  steps. Two concurrent threads pick the same batch.
- **`save_db` is a full-file rewrite** of a ~206K-line JSON. No locking;
  two concurrent saves clobber each other. Typical loss: most-recent
  match-status flip silently reverts.
- **`git_commit_batch` walks the working tree.** Two sessions writing to
  the same `Class.cpp` race on the file before either commits; the
  first session's edits are lost when the second writes.
- **In-loop counters** (`consecutive_failures`, `total_matched`,
  `consecutive_refusals`) are not atomic; under concurrency, the
  circuit breaker reads stale values and may never trip.
- **Single overnight branch shared by all sessions.** Today fine; under
  multi-agent it's still fine if commits go through a single
  serialization point.

### What's missing for A/B analytics

- DB has no `matched_by_backend` / `matched_by_session_id` /
  `matched_at`; the only attribution is the commit message and the
  `match_*.jsonl` event stream. Reconstructing per-attempt history
  requires log mining.
- `failure_notes` lacks backend/model/timestamp.
- No cost/token capture from either backend's stream-JSON output.
- No statistical reporting tool. The TUI scoreboard shows raw counts
  with no confidence intervals, no significance test.

### What works today and shouldn't be broken

- `Backend` ABC + `run_session()` shared loop. Stays.
- Quality gates (pure-asm rejection, extern-C class-method rejection).
  Stay; both extend cleanly to multi-agent.
- The `audit_symbol_signatures.py` framework. Stays; per-attempt
  attribution will plug into its existing flow.
- The TUI's event-stream consumption model. Stays; only the rendering
  layer expands to N panels.

## Architecture

### 1. Concurrency model: thread-pool coordinator

Single orchestrator process. `concurrent.futures.ThreadPoolExecutor(N)`
drives N worker threads, each running one session at a time. Workers
own only their own subprocess pipes and result parsing — they never
touch the DB, the git working tree, or the JSONL log file directly.

Three resources, three coordination mechanisms:

- **DB (`config/functions.json`)**: single `threading.RLock`
  (`db_lock`) on the coordinator thread. `pick_next_batch` +
  `set_batch_status("in_progress")` runs under the lock. Workers
  receive an immutable snapshot of the batch and the relevant fields;
  on completion they hand a results payload back to the coordinator,
  which applies status changes under the lock. Replace the current
  `save_db` (full-file rewrite) with write-to-tmp + `os.replace` so a
  crash mid-write can't corrupt the file.

- **Git commits**: dedicated commit thread drains a `commit_queue`.
  Workers never call `git_run` mutators. Commit thread runs serially —
  one commit at a time, in completion order. Documented bug #4
  (operator-staged-changes absorbed into session commits) is fixed
  here too: commit thread uses `git commit -- <explicit paths>` rather
  than `git commit` with an implicit staged set.

- **Source files**: `class_locks: dict[str, threading.Lock]`. Each
  batch holds its class lock for the full lifetime (pick → spawn →
  verify → commit). The picker filters out classes whose lock is
  currently held — no spinning, no livelock.

The current `Backend.run_session` shape stays unchanged. Workers call
it directly; it already uses 2 reader threads + a queue, all
thread-local.

### 2. Three operating modes

The framework supports three distinct configurations. They share the
same orchestrator, worker pool, and reporting tool — only the function-
to-backend assignment policy and the git-isolation strategy differ.

#### Mode A — pure disjoint (max throughput)

```bash
./tools/run_overnight.sh --backend claude,codex
```

Every function is attempted by **exactly one** backend. The `class_lock`
mechanism guarantees two agents never work on the same class file at
the same time; `in_progress` status guarantees no double-claim. With
stratified random assignment (§4), each backend gets a matched workload
mix — same tier × size × class-context distribution — so per-backend
throughput numbers are fair.

**Use when:** routine overnight production. Doubles total coverage vs
single-backend; fair throughput comparison; no API spend doubling.

**Statistical signal:** rate-difference and CI on independent samples
(two-proportion z, Welch's t on per-session times). NOT McNemar-valid
because no per-function pairing.

#### Mode B — pure shootout (max signal)

```bash
./tools/run_overnight.sh --backend claude,codex --shootout
```

The **same** function list is given to **every** backend. Each backend
runs in its own git worktree (isolated index, isolated working tree)
so neither sees the other's commits. Each function attempted by **both**
→ McNemar's exact test on per-function disagreements.

Results from shootout sessions are written to backend-tagged
`session_results/<sid>.json` files and recorded in `logs/attempts.jsonl`,
but **not committed to a shared branch**. The reporting tool picks the
winning match (or neither, or both) per function offline. Optionally
the operator can merge the chosen winner into the main `overnight/<ts>`
branch with `tools/ab_promote.py` (Phase 3).

**Use when:** benchmark days, model promotion decisions, regression
testing a new backend release.

**API cost:** doubles per function. Not for routine production.

**Statistical signal:** McNemar's exact + per-function effect sizes
+ paired bootstrap on cost/duration.

#### Mode C — hybrid (recommended steady state)

```bash
./tools/run_overnight.sh --backend claude,codex --paired-reserve 50
```

Most of the run is Mode A (stratified-disjoint, max throughput). A
reserved slice of N functions (e.g., 50, stratified across the same
strata as the disjoint pool) is split off and run shootout-style in
isolated worktrees. Each run accumulates ~50 paired data points; over
3-5 runs the McNemar n is enough for promotion-grade verdicts.

**Use when:** routine production AND ongoing benchmark accumulation.
The default once Phase 3 lands.

**API cost:** ~10% overhead vs Mode A (50 paired functions × 2 backends
inside a ~500-function run).

**Statistical signal:** Mode A on the disjoint slice + Mode B on the
reserved slice. The report combines both — disjoint-slice CIs answer
"throughput?", reserved-slice McNemar answers "head-to-head quality?".

### 3. A/B data model

**New file: `logs/attempts.jsonl`**, append-only, one record per
(session, function) attempt:

```
{"ts", "run_id", "session_id", "backend", "model", "variant",
 "address", "name", "class_name", "size", "is_leaf", "obj_file",
 "claimed_status", "verified_status", "verify_reason",
 "rejected_extern_c", "session_duration_s", "session_share_s",
 "input_tokens", "output_tokens", "cached_tokens", "cost_usd",
 "agent_refused", "prep_error", "session_error_kind"}
```

This is the substrate for all A/B analytics. Not stored in
`functions.json` (would balloon a hot-path file).
`tools/ab_attempts_extract.py` rebuilds it from existing logs as a
one-time migration; orchestrator writes to it going forward.

**`functions.json` does gain three small fields** for fast queries:
`matched_by_backend`, `matched_by_session_id`, `matched_at`. These
reflect *who currently owns the match in main*, not the full attempt
history.

**`failure_notes` schema enrichment**: each entry adds `backend`,
`model`, `timestamp`, `variant` alongside the existing
`session`/`notes`. Cap stays at 5 entries.

**Cost capture (gap today)**: both backends emit token usage in their
stream-JSON output (`usage.input_tokens`, `usage.cache_read_input_tokens`,
`usage.output_tokens` for Claude; `token_count` for Codex). Add a
`usage` AgentEvent kind in `base.py` and a `session_usage` log event
written by `run_session`. Without this, "cost per match" is
unmeasurable.

### 4. A/B sampling strategy

The picker is mode-aware. At run start it builds an assignment schedule
based on the chosen mode.

**Mode A (disjoint):** stratified random assignment. Partition the
candidate pool into strata `(tier ∈ {0,1,2,3}) × (size_bucket) ×
(has_class_context)`. Within each stratum, batches are alternately
assigned to each `(backend, model)` identity. Each identity gets a
matched workload mix — no Simpson's-paradox unfairness from one backend
getting all the easy small leaves.

**Mode B (shootout):** the same input list (either all-untried, or
loaded from `--targets` or `--paired-reserve`) is fanned out to every
identity. Each identity gets the *full* list, not a slice; the picker
returns batches in a deterministic order so each backend sees the
batches in the same sequence (controls for any ordering effects in
the agents' context-building).

**Mode C (hybrid):** the picker holds out N functions (`--paired-reserve N`)
as the shootout reserved set, stratified to match the overall pool's
distribution. The remaining pool is scheduled per Mode A. Workers
draw from whichever queue is non-empty; Mode B's worktree-isolated
workers run only against the reserved queue.

**Reject pure random (the current `--backend claude,codex`):**
correlated batches make a single run uninterpretable. The current mode
becomes a fallback once Mode A is the default.

### 5. Statistical reporting

End-of-run `tools/ab_report.py` produces:

- Per-`(backend, model)` metrics with Wilson 95% CIs on rates and
  bootstrap 95% CIs on cost/throughput.
- Stratified tables (so a backend that's better on small leaves doesn't
  get masked by harder big-function failures).
- McNemar's exact test on the paired set.
- A **promotion verdict** row: GREEN if better backend's CI lower bound
  exceeds worse backend's CI upper bound; AMBER on overlap; RED if
  reversed.
- No bare p-values; everything reported with effect size + CI:
  `Δ match-rate = +3.2pp [95% CI +0.8, +5.6], n=92, p<0.01`.

The TUI carries only **lightweight live indicators** — Wilson CI under
each scoreboard row. Live p-values invite peeking-bias; the rigorous
read happens post-run.

### 6. TUI evolution

- **`RunState` grows `AgentSlot[]`** — per-slot identity, current
  session, narrative + tool deques. Existing global outcomes/scoreboard
  stay.
- **Layout adapts to N**:
  - N=1 → current layout, unchanged.
  - N=2..4 → tiled grid, one tile per agent (identity-bordered),
    stacked above shared orch + outcomes.
  - N≥5 → spotlight mode (one big panel, others one-line strips).
- **Identity coloring** uses two axes:
  - Hue = backend family (claude→sun ramp, codex→forest ramp).
  - Shade within family = model rank (4.6 dim, 4.7 sunlit).
- **Event routing**: `session_start` carries `slot_id`; TUI maintains
  `session_id → slot_id` map; events are routed to the correct slot's
  deques by session_id. Unknown session → drop (defensive on replay
  startup).

## Phased migration

The path from where we are to the full design, smallest credible step
each:

### Phase 1 — concurrent dual-backend (smallest end-to-end win)

- Refactor inner loop into `run_one_session(batch, backend)` callable.
- Add `db_lock: RLock` around every read/write of `functions`. The
  current `save_db` becomes write-to-tmp + `os.replace`.
- Wrap picker so it filters out in-progress *classes* (not just
  functions) — `class_locks` dict.
- `ThreadPoolExecutor(max_workers=2)` with `submit()` + `as_completed()`.
- Commits stay on the coordinator thread (serialized by the call site,
  no commit thread yet).
- Keep TUI single-panel; multi-agent events still tagged with
  session_id, just fewer renders.

Outcome: 2 concurrent agents (claude + codex), provably race-free DB
+ class file, zero TUI changes. ~150 lines of refactor.

### Phase 2 — commit thread + per-attempt log

- Extract commit into a thread draining `commit_queue`. Use explicit
  `git commit -- <paths>` (also closes bug #4).
- Add `logs/attempts.jsonl` writes inside `process_session_results`
  and the verify loop.
- Backend layer: `usage` AgentEvent kind + `session_usage` log event.
  `claude.py` reads from the `result` event in stream-JSON;
  `codex.py` reads from its `token_count` event.
- One-shot `tools/ab_attempts_extract.py` to backfill historical
  attempts.jsonl from existing logs.

Outcome: per-attempt history exists, cost data flows. Safe at N=3-4.

### Phase 3 — sampling modes + worktrees + reporting tool

This phase delivers the A/B framework end-to-end: all three operating
modes (disjoint, shootout, hybrid), the schedule pre-computer, the
isolation primitive that shootout requires, and the reporting tool.

- `tools/ab_schedule.py` produces the assignment schedule at run start
  (Mode A stratified, Mode B fan-out, Mode C hold-out + stratify).
- `pick_next_batch` becomes mode-aware: takes a `(backend, model)`
  filter, draws from disjoint queue OR shootout queue depending on
  the worker's assignment.
- **Per-worker git worktrees, gated by mode.** Disjoint runs (Mode A)
  share the main worktree as today (one commit thread serializes the
  writes; class_locks prevent file races). Shootout runs (Mode B) and
  the reserved-slice workers in Mode C **must** have isolated worktrees
  — otherwise both backends' edits to the same `Class.cpp` collide.
  Implemented via `git worktree add overnight-worktree-<slot> <branch>`
  at run start; torn down on run end. The commit thread becomes
  worktree-aware.
- `--shootout` and `--paired-reserve N` CLI flags select Modes B and C.
- `tools/ab_report.py` consumes `attempts.jsonl`. Per-mode analysis:
  - Disjoint slice → two-proportion z, Welch's t, Wilson CIs.
  - Shootout slice → McNemar's exact + paired bootstrap.
  - Mode C combines both reports.
- `tools/ab_promote.py` (optional, ships with this phase) inspects
  shootout results and writes the chosen winner's match into the main
  branch. Manual default — no auto-promote.

Outcome: rigorous A/B numbers from a single run; promotion-grade
verdicts from a 3-5 run aggregate; production throughput preserved by
defaulting to Mode C.

**Why worktrees moved from Phase 5 to here:** shootout is the explicit
trigger. Mode B at N=2 already needs them — two backends both writing
`src/Class.cpp` in the same tree clobber each other regardless of how
many workers there are. The "scale-driven" worktree need (Phase 5
below) is a separate concern.

### Phase 4 — multi-agent TUI

- `RunState.slots` + per-slot deques.
- Tiled layout for N=2..4; spotlight for N≥5.
- Identity 2-axis coloring (backend×model).
- Per-slot live indicators with Wilson CIs.
- `tools/ui/dev/demo_stream.py` multi-agent variant for development.
- Mode-aware status: indicator in the run-status panel showing whether
  this run is disjoint / shootout / hybrid.

Outcome: TUI matches what the backend is actually doing.

### Phase 5 — scale-driven worktree generalization (only if needed)

Phase 3 introduced worktrees for shootout's correctness need. This
phase extends them to ALL workers as a contention-relief measure if
N>4 disjoint runs reveal real lock pressure on the shared worktree's
index. `git worktree add` per worker; commit thread becomes a fan-in
across worktrees. Largest change in the plan; only land if the simpler
shared-tree model demonstrably bottlenecks at higher N.

## Risks

- **API rate limits cluster the failures across workers.** Per-backend
  semaphore (start at 1 for codex, 2 for claude); on rate-limit
  detection, mark the backend "cooling" for 60s and skip in the picker.
- **Same-class livelock**: picker filters out locked classes *before*
  candidate sort, not after. Sleep 5s if no eligible batch.
- **Worker hangs on a stuck child**: outer future timeout
  (`session_timeout + 60s`) force-cancels, releases class lock, reverts
  in_progress.
- **TUI sees interleaved sessions**: ring buffers per slot solve
  narrative chaos; orch_note prefixed `[A#]` when its text references a
  session_id detectable by regex.
- **Statistical noise from small N**: report CIs always; promotion
  verdict requires non-overlapping CIs, not just point estimate
  difference.

## Open questions

These are the things this plan doesn't yet answer; flag them as the
implementation goes.

1. **Schedule fairness when one backend rate-limits.** If codex cools
   for 60s mid-run, its share of the schedule drops. Should the picker
   re-balance to keep the comparison fair, or accept the imbalance and
   weight the analytics by attempt count?

2. **Shootout cost vs value.** Paired runs double API spend on the
   reserved slice. Is 50 functions × 3 runs/week worth ~$X/week for
   McNemar-quality data? Set a budget threshold.

3. **DB schema migration on the running tree.** Adding
   `matched_by_backend` to `functions.json` is a one-time backfill.
   The migration script needs to handle the case where a function
   matched in 2025 doesn't have backend attribution available — leave
   `null`, or assign `claude` (the only backend used pre-codex)?

4. **TUI N>4 ergonomics**: spotlight + collapsed strips is one option;
   tabbed (number-key to switch) is another. No live data on which
   operators actually need yet.

## What this doc replaces / supersedes

- The single-backend assumption embedded in `tools/orchestrator.py`'s
  inner loop.
- The ad-hoc `--backend claude,codex` random-pick A/B (kept as a
  simpler mode for short runs, but the rigorous path is the
  scheduled+stratified one).
- The single-axis backend color scheme in the TUI.

When all five phases land, this doc gets `[done]` markers (per the
direction-doc convention in CLAUDE.md) and a one-line summary at the
top.
