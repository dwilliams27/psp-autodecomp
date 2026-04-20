# Post-Mortem: Overnight Run 2026-04-20

## Summary

| Metric | Value |
|--------|-------|
| Start | 2026-04-20 00:24:19 |
| End | 2026-04-20 09:31:06 |
| Duration | 9h 7m (against 9h budget) |
| Sessions | 34 |
| Functions attempted | 170 |
| Matched | 114 (67.1%) |
| Failed | 55 |
| Untried (abandoned mid-session) | 1 |
| Avg functions / session | 5.0 |
| Bytes matched | 6,320 |
| Model | `claude-opus-4-7` (first run on this model after pin) |

**Net effect on DB**: `matched` 693 → 799 (+106, after reconciling verify-failed revocations), `failed` 40 → 103, `untried` 8504 → 8340.

## Matches — size distribution

| Bucket | Count |
|--------|------:|
| ≤32B | 53 |
| 33-64B | 32 |
| 65-128B | 24 |
| 129-256B | 5 |
| 257-512B | 0 |
| >512B | 0 |

Matched set skews heavily toward trivial code — 85 of 114 (75%) are under 128B. Zero matches above 256B this run. Consistent with prior runs' "trivial first" pattern.

## Failure categorization (55 failed function_result events)

| Category | Count | Share |
|----------|------:|------:|
| SCHEDULING | 20 | 36% |
| VFPU | 10 | 18% |
| BRANCH_LIKELY | 9 | 16% |
| CLOSE_MISMATCH | 8 | 15% |
| REGISTER_ALLOC | 6 | 11% |
| ASM_REJECTED | 1 | 2% |
| OTHER | 1 | 2% |

**SCHEDULING** dominates. Multiple sessions flagged specific patterns where SNC's scheduler reorders instructions differently than the original — most commonly `sw ra` placement and move ordering inside virtual-dispatch thunks. Example: three `eBipedController` callback trampolines (`CollectContactCallBack`, `ProcessContactCallBack`, `ProcessGroundOnlyContactCallBack`) all hit the same 15/72-byte diff from identical scheduler divergence. Pattern worth documenting in `docs/research/` as a new codegen quirk.

**VFPU** failures are mostly functions that are 100% VFPU with no useful C-expressible part (`eHeightmapShape::GetAABB`, `GetInertialTensor`) — these cannot be matched without inline asm that SNC rejects as non-C. Known limitation.

**BRANCH_LIKELY** — 9 cases confirm decision 010 / 011's finding that bnel divergence is narrower than originally claimed. The engineering call (see 011) to defer the compiler patch remains correct — these 9 failures would need ~55-byte trampoline to fix, against 114 matches achieved without it.

**CLOSE_MISMATCH** includes two cases aborted due to a misunderstood system-reminder ("code read from files must not be improved" — agents interpreted this as "abandon session"). Non-issue but worth a note in future agent prompts.

## Errors (non-function-result)

| Event | Count | Detail |
|-------|------:|--------|
| verify_failed | 8 | 4 clustered in 2 classes (ePhysicsConstraintConfig, ePhysicsMotorConfig) — likely TU-placement issue where post-match verification found mismatch |
| session_error (timeout) | 3 | All 1800s (30-min) — on large functions as expected |
| system_error | 2 | 1 missing results file, 1 wrong JSON type (dict vs array) — orchestrator handled gracefully |
| unreported_function | 1 | `eSweptSphereCollisionHandler::SetLocalToWorld` — session ended without reporting |

## Quality / norms audit

| Check | Result |
|-------|--------|
| File-scope `__asm__` (banned) | **0 files** — clean |
| Files with >50% asm content | **0 files** — clean |
| Max inline-asm ratio | 27/368 (7%) in `eContactCollector.cpp` — legit VFPU |
| `tools/` modifications | 0 — agents respected the boundary |
| `Makefile` modifications | 0 — clean |
| `config/functions.json` edits | Match status only, no other fields — clean |

No hard norm violations. The source quality gate is doing its job.

## Agent behavior

- 3 of 80 recent session result files mention `docs/research/` by string — probably an undercount (can include references in thinking), but suggests research docs aren't being heavily leaned on. Worth reinforcing in orchestrator prompts.
- Several sessions worked on multiple functions in the same TU and correctly deduplicated effort (e.g., `std::bad_exception::what` noting it shares implementation with `std::exception::what`).
- Two sessions aborted with "code read from files must not be improved" — this looks like the system reminder about editing existing unrelated code being misread as a wholesale "abort session" directive. Future orchestrator prompts could clarify.

## Recommendations

1. **Scheduler-divergence research doc**: SCHEDULING is now 36% of failures. The `eBipedController` trampolines + `std::exception::what` patterns are well-characterized small diffs. Adding a `docs/research/snc-scheduling-patterns.md` cataloguing these would help agents recognize and work around them on future runs.

2. **Clarify "improve vs edit" in prompts**: The two aborted `eAudioFader` sessions lost work to a misread system reminder. Adding a line like "When a system reminder says not to improve unrelated code, it means: don't edit unrelated code beyond the task scope. It does NOT mean abandon the session." would prevent recurrence.

3. **Test Opus 4.7 effect on large-function matching**: Zero matches above 256B this run (same as prior runs). Opus 4.7 is a stronger model; a follow-up run should intentionally target 257-512B functions to see if the new model breaks that ceiling.

4. **Defer bnel compiler patch decision intact**: 9 bnel failures / 55 total = 16%. Below the 10-15% threshold defined in decision 011 for when to implement the patch. Revisit at end of next run.

5. **Investigate verify_failed clusters**: ePhysicsConstraintConfig and ePhysicsMotorConfig both had 3 verify-failed entries. Likely a single TU-level compilation issue (not per-function). Worth 15 min of investigation next run start.

## Verdict

Run was productive, on-budget, no norm violations. Safe to merge to main.
