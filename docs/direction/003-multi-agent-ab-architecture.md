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

### 2. Disjoint vs overlapping batches

**Default mode: disjoint.** The `class_lock` mechanism guarantees two
agents never work on the same class file at the same time. `in_progress`
status guarantees two agents never claim the same function. This is
what production overnight runs should use.

**Separate `--shootout` mode** for paired A/B: the same fixed function
list is given to each backend, in isolated git worktrees, so they don't
see each other's commits. Results are written to backend-tagged
session-results files, **not committed to the same branch**. Shootout
output is consumed only by the A/B reporting tool, never merged to
main. This is the only way to get a McNemar-valid paired comparison
on per-function quality.

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

**Default: stratified random assignment.** At run start, partition the
candidate pool into strata: `(tier ∈ {0,1,2,3}) × (size_bucket) ×
(has_class_context)`. Within each stratum, batches are alternately
assigned to each `(backend, model)` identity. Each identity gets a
matched workload mix — no Simpson's-paradox unfairness from one backend
getting all the easy small leaves.

**Plus: paired hard-set**. A small reserved slice (~50 functions,
stratified) is run by *both* identities in shootout mode. This is the
McNemar-valid paired-comparison input.

**Reject pure random (current):** correlated batches make a single 4h
run uninterpretable.

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

### Phase 3 — stratified sampling + reporting tool

- `tools/ab_schedule.py` produces a stratified-paired schedule at
  run start.
- `pick_next_batch` gains a `(backend, model)` filter that respects the
  schedule.
- `tools/ab_report.py` consumes `attempts.jsonl`, produces end-of-run
  markdown summary + multi-run aggregate.
- `--shootout` flag wires up the paired hard-set in isolated git
  worktrees.

Outcome: rigorous A/B numbers from a single run; promotion-quality
data from a 3-run aggregate.

### Phase 4 — multi-agent TUI

- `RunState.slots` + per-slot deques.
- Tiled layout for N=2..4; spotlight for N≥5.
- Identity 2-axis coloring (backend×model).
- Per-slot live indicators with Wilson CIs.
- `tools/ui/dev/demo_stream.py` multi-agent variant for development.

Outcome: TUI matches what the backend is actually doing.

### Phase 5 — git worktrees if needed

Defer until concurrency >4 reveals real contention. `git worktree add`
gives each worker an isolated index sharing one repo. Largest change in
the plan; only land if the simpler model demonstrably bottlenecks.

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
