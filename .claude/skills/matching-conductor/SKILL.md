---
name: matching-conductor
description: Run a self-managed, time-boxed byte-exact matching session for psp-autodecomp by conducting parallel workflow-engine rounds (NOT the overnight harness). Use when the operator says "run an N-hour matching session", "convert compute into matches for N hours", or asks to resume/repeat the self-managed conductor run. Drives target selection, the parallel match engine, a strict harvest+asm-audit gate, /pre-commit-review, and batched merges to main.
---

# Matching Conductor

A self-managed loop that turns compute into byte-exact matches for a fixed wall-clock
window. You (the interactive assistant) are the conductor: select a slice of independent
targets, launch the parallel match engine on them, then harvest — re-verify, audit for
gaming, review, commit, credit, merge — and repeat until the window closes, ending with a
clean wind-down. This is NOT `tools/orchestrator.py` / `run_overnight.sh` (that needs the
sandbox + operator sudo); this runs entirely in your session.

Born from the 2026-06-01 run (+134 matches, 5300→5434, 59.0%) — see
`docs/sessions/2026-06-01.md` for the worked example and lessons.

## Pieces (all committed)

- **Engine:** `.claude/workflows/overnight_match_parallel.js` — attacks N independent
  split-TU targets in parallel (one deep agent each). Its agent prompt already carries the
  research-doc routing, the ADR-012 source-coloring toolkit (step 6b), and the gaming
  ban-list. Launch via the `Workflow` tool with `scriptPath` + `args` = the JSON target array.
- **Selector:** `tools/select_match_targets.py` — emits that JSON array (and reserves the
  picks in `logs/overnight_excluded.txt`). Modes `fresh` and `regalloc`. See `--help`.
- **Verifier (oracle):** `tools/compare_func.py <src> --no-update-db` — the ONLY source of
  truth for a match. `--update-db` credits it.
- **Review:** `/pre-commit-review` before every commit.

## Setup (once per session)

1. Confirm a clean tree on `main` (or create/checkout the run branch):
   `git status --short --branch`. Work on a branch like `wf-match-YYYY-MM-DD`; merge to
   `main` in batches.
2. Compute the end epoch: `END=$(( $(date +%s) + HOURS*3600 ))`. Every round, recompute
   `REMAIN_MIN=$(( (END - $(date +%s)) / 60 ))`.
3. Drive the loop with `/loop` (dynamic mode) + `ScheduleWakeup`: each round's work runs,
   then you `ScheduleWakeup` a hang-backstop (~1500–1800s) and pass the same conductor
   prompt back. The engine's completion `<task-notification>` is the real wake signal; the
   backstop only covers a hung round. Keep the full state (round #, engine task-id, merge
   cadence, END epoch) in the loop prompt so it survives context compaction.

## Round cadence

- Default each round = **14 targets**. Rounds are independent (distinct new `src/*.cpp`).
- Mostly **fresh** rounds, climbing the size band: start small and raise `--min-size` each
  round (108→120→132→148→160→176→…). Sibling families generalize hard — a matched
  `Foo::Bar` makes `Foo::Baz`/the whole class far easier, so fresh bands rich in siblings
  pay off.
- Every **~3rd round = a `regalloc` batch** (the failed pool): exact-size near-misses with
  register/scheduler coloring residuals that the step-6b toolkit + reg-aware permuter can
  sometimes close. Yield is lower (~30–50%) and rounds run longer (permuter passes).
- **ff-merge to `main` every ~3 rounds** (and always at wind-down) so banked work is safe.

## Per-round procedure

### 1. Select + launch

```bash
# fresh round (climb the band each round via --min-size)
python3 tools/select_match_targets.py --mode fresh --count 14 --min-size <floor> --out /tmp/slice.json
# every ~3rd round instead:
python3 tools/select_match_targets.py --mode regalloc --count 14 --out /tmp/slice.json
```

Read `/tmp/slice.json`; optionally add one-line sibling hints to `failure_notes` (e.g.
"sibling of matched X — mirror it"). Then launch the engine with the `Workflow` tool:
`Workflow({ scriptPath: ".claude/workflows/overnight_match_parallel.js", args: <the array> })`.
Then `ScheduleWakeup` the backstop and end the turn.

### 2. Harvest (when the engine's task-notification fires)

For EACH result with `status == "matched"`:

a. **Re-verify against the oracle** — `compare_func.py <src> --no-update-db`. Commit ONLY
   if it itself reports `MATCH` / 0-diff. **NEVER trust the permuter's "exact"** — it has
   masked register-field bytes and reported false-EXACT (2026-06-01 R10/R11).

b. **ASM-content audit** (reject/HOLD even if byte-exact — these are gaming, not matches):
   - `__attribute__((naked))` whole-body asm
   - `register T __asm__("$reg")` register pinning
   - hand-written `jr`/`b`/`jal` or other control flow in inline asm
   - hardcoded `$a0–$a3` argument-register reads
   - integer-MIPS asm materializing a C-expressible value (e.g. `ori %0,$0,1` to force a
     constant) — test by mentally replacing with plain C
   - synthetic **`volatile` dead-store / dummy-local / no-op self-write** crutch
     (`volatile int scratch=0;`, a `volatile` local accumulator, `x += 0`, `x |= 0`) — a
     statement with no genuine program semantics inserted only to emit a matching byte
   - asm-jr scheduler-seed predecessors; non-trap `.word` logic
   - **ALLOWED:** constraint-bound VFPU/FPU ops with no C equivalent using `%0/%1` operands
     (`vzero.t`/`sv.q`/`vcmp`/`mfvc`/`vdot`); `v4sf`/native vector types; empty
     `__asm__ volatile("" ::: "memory")` / `"+r"(x)` / `"r"(x)` scheduler barriers (emit NO
     instruction); `.word 0x1000ffff` as the genuine function tail-trap; `__asm__("symbol")`
     rename labels; fdlibm union type-pun.
   Held results are removed (`rm` the file), not committed; record them in the journal.

c. **`/pre-commit-review`** the clean batch (one combined pass for 0-asm files; the asm
   dimension matters most here). Fix or skip findings.

d. **Commit + credit + merge:**
   ```bash
   git add <clean src files> config/functions.json
   git commit -m "wf-run R<N>: <k> byte-exact matches (<band/type>) ..."   # note held results
   for f in <clean files>; do python3 tools/compare_func.py "$f" --update-db; done
   ```
   ff-merge to `main` on the ~3-round cadence:
   `git checkout main && git merge --ff-only <branch> && git checkout <branch>`.

`near_miss` / `failed` / `blocked` results are left as untracked scratch (harmless;
`config/functions.json` status already excludes banked work from future selection).

### 3. Next round

Recompute `REMAIN_MIN`. **If `REMAIN_MIN` < ~35, do the wind-down instead of launching.**
Otherwise pick the next slice (raise the band, or a regalloc batch on cadence) and loop.

## Wind-down (window closing)

1. `git status --short` — clean tree on the branch (untracked agent scratch is fine; no
   tracked dirty files).
2. Append a session journal `docs/sessions/YYYY-MM-DD.md`: outcome (DB before→after, %,
   `+N`), per-round table, the **held-gaming list**, files-to-reconcile (pre-existing
   naked-asm / register-pin submissions; shared-TU duplicate symbols), the permuter
   false-EXACT caveat, residual unmatchable classes, and any tooling changes. Use
   `docs/sessions/2026-06-01.md` as the template.
3. Final ff-merge to `main`. `git log --oneline -25`.
4. `PushNotification` a one-line summary; then STOP (omit `ScheduleWakeup`).

## Known unmatchable / blocked classes (don't burn rounds on these)

The selector already excludes them, but for awareness: **VisitReferences** (ADR-014
mangling), **va_start family** (printf/scanf/cStr*/varargs ctors — `__builtin_stdarg_start`
crashes wibo+pspsnc), **Collide** dispatchers (ADR-012 Mechanism-A, real-TU reconstruction
only), **libgcs** soft-float, `sce*` SDK stubs (null obj). Genuinely-resistant residuals
that are NOT selector-excluded but recur: ADR-006 branch-likely over/under-emission, the
trailing-`b .` self-trap idiom, VFPU `jr`-delay-slot sink, and TU-context register-allocation
/ frame-size (ADR-012 Mechanism-B) coloring residuals. Mark these `near_miss`/`failed` and
move on — never reach for a gaming crutch to force them.

## Norms (non-negotiable)

Byte-exact (masked-relocation) is the only standard. No assembly-as-match, no codegen
crutches — the north star is "what a respected matching-decomp project would do." Agents
create exactly one new `src/*.cpp` per target and never edit `tools/`, `config/`,
`include/*.h`, `Makefile`, or shared TUs. `/pre-commit-review` before every commit.
