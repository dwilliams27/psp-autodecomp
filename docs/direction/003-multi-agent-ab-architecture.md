# Multi-agent concurrent matching + robust A/B benchmarking

**Status:** Phases 1-3 complete and in production. Phase 4 (TUI) is next.
Begun 2026-04-25; Phase 3 landed 2026-04-26.

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

## Current state (updated 2026-04-26)

Phases 1-3 are implemented and running in production. The orchestrator
runs N concurrent workers via `ThreadPoolExecutor`, with `db_lock`,
per-session file ledgers, class locks, a commit thread, per-attempt
logging (`logs/attempts.jsonl`), cost capture, and all three operating
modes (A/B/C). First Mode C production run: 2026-04-26
(`--paired-reserve 50`, claude + codex, 2 workers + 2 shootout slots).

Per-backend circuit breaker landed 2026-04-26: one dying backend no
longer kills a healthy run. Verifier bug affecting 9.9% of functions
(templates, nested classes) fixed same day.

**Remaining:** Phase 4 (multi-agent TUI) and Phase 5 (scale-driven
worktree generalization, only if needed).

## Architecture

### 1. Concurrency model: thread-pool coordinator

Single orchestrator process. `concurrent.futures.ThreadPoolExecutor(N)`
drives N worker threads, each running one session at a time. Workers
own only their own subprocess pipes and result parsing — they never
touch the DB, the git working tree, or the JSONL log file directly.

Three resources, three coordination mechanisms:

- **DB (`config/functions.json`)**: single `threading.Lock` (`db_lock`)
  on the coordinator thread. Plain `Lock`, not `RLock`: only the
  coordinator touches the DB, no recursive acquisition. Workers
  receive an immutable snapshot of the batch and the relevant fields;
  on completion they hand a results payload back to the coordinator,
  which applies status changes under the lock. Replace the current
  `save_db` (full-file rewrite) with write-to-tmp + `os.replace` so a
  crash mid-write can't corrupt the file.

- **Git commits**: dedicated commit thread drains a `commit_queue`.
  Workers never call `git_run` mutators. Commit thread runs serially —
  one commit at a time, in completion order. Each commit stages
  exactly the files in the session's *declared file ledger* (see
  below) and runs `git commit -- <ledger paths>`. The
  `_session_dirty_paths()` global tree-walk goes away; bug #4 is
  closed by removing the offending mechanism, not by tweaking the
  commit command.

- **Source-file ownership: per-session file ledger**. The picker
  computes an *allowed file set* for each batch when it picks the
  batch — `{src/<Class>.cpp, include/<Class>.h, src/<Class>_*.cpp}`,
  optionally extended by inspecting the function's call-graph
  callees' headers if the agent's prompt indicates it'll need to edit
  them. The worker is told (in the prompt) that it may only modify
  files in this set. After the agent completes, the worker captures
  `git status --porcelain` and partitions:
  - **Files in the allowed set, dirty** → ledger; pass to commit thread.
  - **Files outside the allowed set, dirty** → quality-gate rejection.
    Either the agent strayed or the session collided with another
    worker's allowed set (which class_locks should prevent). Match is
    marked `failed` with a clear note; out-of-scope edits are reverted.

- **Class-level lock** (`class_locks: dict[str, threading.Lock]`):
  derived from the allowed file set. Picker filters out classes whose
  lock is currently held — no spinning, no livelock. Headers in the
  `include/` set need their own locks so two batches that both depend
  on `include/cBase.h` don't write it concurrently. In practice
  `cBase.h`, `cMemPool.h`, and a few other broadly-used headers
  become contention hot-spots; the worker that holds the lock first
  wins, the other worker either waits (for short edits) or gets
  rejected by the file-ledger check (for collisions).

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

**`functions.json` does gain four small fields** for fast queries:
`matched_by_backend`, `matched_by_session_id`, `matched_by_model`,
`matched_at`. These reflect *who currently owns the match in main*,
not the full attempt history.

**Migration backfill** for the ~1240 already-matched entries:
- All matched entries that pre-date the codex backend get
  `matched_by_backend = "claude"` (codex commits are date-bounded;
  trivial to identify by author/`Co-Authored-By` line).
- Model attribution uses the git history of `tools/common.py` /
  `tools/orchestrator.py`. Commit `41a4601` (2026-04-20) pinned
  the orchestrator to opus-4-7 with the title *"Pin overnight
  orchestrator to Opus 4.7"*. Matches committed before that → best-guess
  `matched_by_model = "claude-opus-4-6"` (Anthropic's default at the
  time, no explicit pin). Matches committed at-or-after → `claude-opus-4-7`.
- `matched_by_session_id` and `matched_at` come from the commit
  message (`session XXXX`) and commit timestamp.
- Codex matches: backend `codex`, model `gpt-5.5`, identified by
  `41a4601`-or-later commits whose committer is `autodecomp` and whose
  date is after the codex backend landed (`777bf8e`, 2026-04-23).

The migration script (`tools/migrations/backfill_match_attribution.py`)
walks the git log, parses commit messages, and writes the four fields
into `functions.json` with one final atomic save.

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

### Phase 1 — concurrent dual-backend (smallest end-to-end win) [x]

This phase is the prerequisite for everything else. Race-freedom is
the gate, not throughput.

- Refactor the inner session loop into `run_one_session(batch, backend)`
  callable.
- Add `db_lock: Lock` around every read/write of `functions`.
  Replace `save_db` (full-file rewrite) with write-to-tmp +
  `os.replace`.
- **Per-session file ledger replaces `_session_dirty_paths()`.** When
  the picker assigns a batch, it computes the allowed file set
  (`{src/<Class>.cpp, include/<Class>.h, src/<Class>_*.cpp}`). The
  worker captures `git status --porcelain` after the agent finishes,
  partitions dirty files into in-set (the ledger) and out-of-set
  (rejection). Only the ledger gets staged + committed. Out-of-set
  edits are reverted (`git checkout -- <path>`) and the match is
  marked `failed` with a clear note.
- `class_locks` dict: per-class lock acquired by the picker for each
  batch's lifetime (pick → spawn → verify → commit → release). Picker
  filters out classes whose lock is currently held.
- **Header lock pool**: same `class_locks` dict also keys the headers
  in the allowed file set. `include/cBase.h` and a few other widely-
  shared headers will be contended; the picker holds them per-batch
  and other batches needing them wait or get filtered.
- `ThreadPoolExecutor(max_workers=2)` with `submit()` + `as_completed()`.
- Commits stay on the coordinator thread for now (serialized by the
  call site; commit thread arrives in Phase 2).
- TUI stays single-panel; multi-agent events still tagged by
  session_id, the rendering layer just sees one session at a time
  visually (Phase 4 fixes that).

**Phase 1 race-freedom invariants** (these are the testable claims):
1. Every modification of `functions.json` happens under `db_lock`.
2. Every modification of `src/<Class>.cpp` or `include/<Class>.h`
   happens by exactly one worker, the one holding `class_locks[Class]`
   for the duration.
3. Every `git commit` covers exactly one session's ledger; no
   cross-session attribution leakage.
4. Out-of-scope file modifications are detected and rejected; they
   never reach a commit.

Outcome: 2 concurrent agents (claude + codex), provably race-free DB
+ class files, zero TUI changes. ~250 lines of refactor (revised up
from ~150 — the file-ledger work is non-trivial).

### Phase 2 — commit thread + per-attempt log + cost capture [x]

- Extract commit into a thread draining `commit_queue`. Each commit
  stages exactly the session's file ledger (Phase 1) via
  `git commit -- <ledger paths>`. Bug #4 is fully closed: there's no
  global tree-walk anymore.
- Add `logs/attempts.jsonl` writes inside `process_session_results`
  and the verify loop.
- Backend layer: `usage` AgentEvent kind + `session_usage` log event.
  `claude.py` reads from the `result` event in stream-JSON
  (`usage.input_tokens`, `usage.output_tokens`,
  `usage.cache_read_input_tokens`, `total_cost_usd`); `codex.py` reads
  from its `token_count` event. Multi-turn sessions accumulate token
  counts across turns; the final `session_usage` event is the sum.
- `tools/migrations/backfill_match_attribution.py` writes the four
  new `matched_by_*` fields onto existing matched DB entries, using
  git history (commit `41a4601` as the opus-4-6 → opus-4-7 cutoff).
- One-shot `tools/ab_attempts_extract.py` to backfill historical
  `attempts.jsonl` from existing `match_*.jsonl` logs.

Outcome: per-attempt history exists, cost data flows, attribution is
queryable in the DB without log mining. Safe at N=3-4.

### Phase 3 — sampling modes + worktrees + reporting tool [x]

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
  writes; class_locks + the file ledger prevent file races). Shootout
  runs (Mode B) and the reserved-slice workers in Mode C **must** have
  isolated worktrees — otherwise both backends' edits to the same
  `Class.cpp` collide.

  **Branch layout for shootout:**
  - Main run branch: `overnight/<ts>` (as today).
  - Per-`(backend, model)` shootout branch: `overnight/<ts>/<backend>-<modeltag>`,
    e.g. `overnight/20260425-220000/codex-gpt-5_5`. Created from the
    main run-branch HEAD at run start.
  - Each shootout worker's worktree:
    `overnight-worktree-<backend>-<modeltag>` checked out at the
    shootout branch.
  - Shootout commits land on the shootout branches only — never on
    the main run-branch. The main run-branch only ever sees disjoint
    commits and (later) promoted winners.

  Worktrees are torn down on run end (`git worktree remove`).
- `--shootout` and `--paired-reserve N` CLI flags select Modes B and C.
- `tools/ab_report.py` consumes `attempts.jsonl`. Per-mode analysis:
  - Disjoint slice → two-proportion z, Welch's t, Wilson CIs.
  - Shootout slice → McNemar's exact + paired bootstrap.
  - Mode C combines both reports.
- `tools/ab_promote.py` (ships with this phase). Reads shootout
  results from `attempts.jsonl` and the per-backend shootout branches.
  For each function in the reserved/shootout set:
  - Both backends matched → operator picks (or `--prefer claude`
    flag); chosen backend's commit is `git cherry-pick`-ed onto the
    main run-branch.
  - Exactly one backend matched → that one's commit is cherry-picked.
  - Neither matched → function stays untried; both shootout branches
    keep their `failed` records for analytics.
  - The cherry-pick brings both the src changes AND the
    `functions.json` updates the shootout commit produced. The
    coordinator re-applies its `db_lock`-guarded merge to ensure no
    duplicate `matched_by_*` writes if the function got cherry-picked
    twice. Default operation: manual; the tool prints a summary and
    asks for confirmation. `--auto-prefer <backend>` flag for batched
    promotion when the operator is okay with a default winner.

Outcome: rigorous A/B numbers from a single run; promotion-grade
verdicts from a 3-5 run aggregate; production throughput preserved by
defaulting to Mode C.

**Why worktrees moved from Phase 5 to here:** shootout is the explicit
trigger. Mode B at N=2 already needs them — two backends both writing
`src/Class.cpp` in the same tree clobber each other regardless of how
many workers there are. The "scale-driven" worktree need (Phase 5
below) is a separate concern.

### Phase 4 — multi-agent TUI

Phase 4 has no hard dependency on Phase 3 — it could ship before
or in parallel with it. The dependency is on Phase 1 only (the
backend emits per-session events). Order based on whichever is the
operator-visibility bottleneck at the time.

- `RunState.slots` + per-slot deques.
- Tiled layout for N=1..4. The first 4 slots get tiles; if N>4, slots
  5+ are pooled into a single one-line "+N more" status strip at the
  bottom of the tile area, listing each as
  `[A5 codex/5.5] on: foo() · 2m12s`. No spotlight mode in this phase
  per direction; revisit only if operators ask.
- Identity 2-axis coloring (backend×model).
- Per-slot live indicators with Wilson CIs under each scoreboard row.
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
  detection, mark the backend "cooling" for 60s and skip in the
  picker. Simpler-is-better resolution adopted: when one backend
  cools, its share drops and `ab_report.py` weights its metrics by
  attempts attempted, not by allocated share. No re-balancing logic.
- **Same-class livelock**: picker filters out locked classes *before*
  candidate sort, not after. Sleep 5s if no eligible batch.
- **Worker hangs on a stuck child**: outer future timeout
  (`session_timeout + 60s`) force-cancels, releases class + header
  locks, reverts in_progress, reverts any uncommitted ledger files.
- **Cross-class header dependencies.** `Foo::method` may need to add
  a forward decl to `include/Bar.h`. Class-level locking on Bar
  doesn't protect because the active worker holds Foo's lock.
  Mitigation: the file ledger includes `include/Bar.h` if it's in
  Foo's call-graph callees' header set (computed at pick time), and
  the corresponding header lock is acquired alongside the class lock.
  Headers shared by many classes (`include/cBase.h`, `include/cMemPool.h`)
  become serialization points — acceptable, since header edits are
  rare in matched sessions.
- **Thin strata in stratified assignment.** A stratum with 1 function
  can't be split between backends; it'd go to whichever drew first.
  Mitigation: at schedule-build time, merge any stratum with <4
  functions into the next-larger size_bucket. Documented as a
  schedule-builder behavior, not a separate fix.
- **Multi-turn cost capture**. Both backends emit per-turn token
  usage. The orchestrator accumulates across turns in
  `Backend.run_session` and emits one `session_usage` event at
  session end with the sum. `cost_usd` is computed from the model's
  rate card (claude: $/Mtok input + $/Mtok output + cache discount;
  codex: similar). Rate cards live in `tools/backends/<name>.py` as a
  small const. Multi-turn detail isn't preserved in `attempts.jsonl`
  — only the summed totals. If per-turn analysis is later needed, a
  separate `turns.jsonl` can be added.
- **TUI sees interleaved sessions**: ring buffers per slot solve
  narrative chaos; orch_note prefixed `[A#]` when its text references a
  session_id detectable by regex.
- **Statistical noise from small N**: report CIs always; promotion
  verdict requires non-overlapping CIs, not just point estimate
  difference.

## Open questions

These are the things this plan doesn't yet answer; flag them as the
implementation goes.

1. **Phase 4 vs Phase 3 ordering.** Phase 4 (TUI) has no technical
   dependency on Phase 3 (modes + worktrees). They can ship in either
   order or in parallel. Decide based on whichever is the
   operator-visibility bottleneck at the time.

2. **Header lock granularity in practice.** Whether `include/cBase.h`
   becomes a contention bottleneck at N=2 or whether class-method
   matches rarely touch it. Re-evaluate after Phase 1 is running for a
   few sessions; if shared-header contention is rare in real workload,
   the plan stands. If common, consider a finer-grained mechanism
   (per-symbol locks within a header, or copy-on-write headers per
   session merged at commit).

3. **`ab_promote.py` cherry-pick conflicts.** A cherry-pick from a
   shootout branch could conflict if the same function was also
   matched on the disjoint slice (shouldn't normally happen — schedule
   keeps them disjoint — but defensive). On conflict, the tool aborts
   with a clear error and asks the operator to pick manually.

### Decisions made (resolved by operator, not yet implemented)

- **Schedule fairness on rate-limit**: simpler — accept imbalance,
  weight analytics by attempts attempted.
- **Shootout cost**: not a constraint; subscriptions cover usage.
  Drop the budget threshold concern.
- **DB migration null backend**: backfill all pre-codex matches as
  `claude`. For model attribution, use commit `41a4601` (2026-04-20
  "Pin overnight orchestrator to Opus 4.7") as the cutoff: pre-cutoff
  matches → `claude-opus-4-6`, post-cutoff → `claude-opus-4-7`.
- **TUI N>4**: show first 4 in tiles, additional slots collapse into
  a single one-line "+N more" strip. No spotlight, no tabs.

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
