# Post-Mortem: Overnight Run 2026-05-05 (dense targets)

**Run ID:** `a9aaec7a288f`
**Branch:** `overnight/20260505-232432`
**Log:** `logs/match_20260505_232432.jsonl`
**Targets:** `config/overnight_targets_20260505_dense.json` (183 functions)

## Repo Integrity

| Check | Result |
|-------|--------|
| `verify_matches.py` exit code | 0, but **5 byte-mismatch problems** in DB |
| All `src/*.cpp` compile | ✓ 0 broken |
| Matched entries with provenance | ✗ 1 missing `symbol_name` (preexisting from 2026-05-03) |
| Session-results ↔ DB consistency | ✓ (2 verify-rejected matches reverted as designed) |
| Working tree clean | ✗ `config/functions.json` dirty (orchestrator died mid-cleanup) |

### BROKEN

`verify_matches.py` reports 5 DB entries claim `matched` but no longer byte-verify (8-byte diff each):

| Address | Function | src_file | Touched tonight? |
|---------|----------|----------|------------------|
| `0x000e4f94` | `gcUIDialog::OnMemPoolReset` | `src/gcUIDialog.cpp` | **Yes — session 4b7556f0** |
| `0x000e503c` | `gcUIDialog::PausesGame` | `src/gcUIDialog.cpp` | **Yes — session 4b7556f0** |
| `0x00047f80` | `eDynamicModel::SetSkin` | `src/eDynamicModel.cpp` | Likely — session 888688e8 added new code in same TU + header |
| `0x0013fbe8` | `gcPartialBodyControllerTemplate::Write` | not edited tonight | Preexisting drift |
| `0x001f4920` | `eMeshShape::GetType` | not edited tonight | Preexisting drift |

**Direct causation evidence (gcUIDialog.cpp):** session 4b7556f0 added a barrier inside `OnMemPoolReset`'s `mEvent1==0` branch and changed `(unsigned char)` → `(char)` plus reordered locals in `PausesGame` — both modifications were attempts to influence reg alloc on a *different* match target in the same TU, but they invalidated the previously byte-exact siblings.

**eDynamicModel.cpp:** the diff between pre-run and post-run is purely additive (new methods `ResetAnimationState`, `SetGeomFlagsOnOff`), but `eDynamicModel.h` got 4 new method declarations injected mid-class. SetSkin's 8-byte diff is consistent with a class-layout / vtable-emission shift.

**Preexisting drift (Write, GetType):** matched_at = 2026-04-27 / 2026-04-29, neither file edited tonight. They were already broken before this run started — the 2026-04-22 audit batch is the most recent post-mortem and it didn't run verify_matches.py end-to-end either.

**Other DB hygiene:**
- 6 entries stuck at `match_status: "in_progress"` (orchestrator was mid-cleanup when it shut down):
  - `0x0004ea6c eMeshShapeNode::Read`
  - `0x000520b8 eHeightmapTemplate::PlatformFree`
  - `0x00136338 gcMsgAckPlayerState::Write`
  - `0x0025fbc8 gcDesiredObjectT<…>::*`
  - `0x002856cc gcMsgAttachEntity::New`
  - `0x002858b0 gcMsgAckPlayerState::New`
- 1 matched entry missing `symbol_name`: `0x001281c0 gcDesiredEntityTemplate::GetDesiredType` (preexisting, manual exemplar match from 2026-05-03 commit `b760e69`).

**Recommended action before next run:**
1. Decide on the 3 tonight-broken matches: revert this run's edits to those two source files (+ headers), or accept the regression and re-match the broken siblings as new work.
2. Investigate the 2 preexisting-drift entries (Write, GetType) separately — they're a different bug.
3. Reset the 6 `in_progress` stragglers to `untried`/`failed` (commit the existing functions.json diff or run a cleanup script).

## Summary

| Metric | Value |
|--------|-------|
| Duration | 5h 56m (full budget used) |
| Workers | 2 |
| Sessions | 21 |
| Functions claimed matched (session_results) | 17 |
| Functions verified matched (orchestrator) | 15 |
| Verify-rejected (claimed but compile broken after revert) | 2 |
| Functions failed | 27 |
| Match rate (verified / attempted) | 35.7% (15 / 42) |
| Ledger reverts (whole sessions wasted) | 9 / 21 = **43%** |
| Out-of-scope edit events | 4 |
| Unreported-function events | 4 (cFactory MarkForClean+CleanGroups reported by 2 sessions but neither produced a match) |
| Avg tool calls / session | 102 (median 97, range 45–185) |
| Avg compare_func runs / session | 18.8 |
| Avg Write (src iterations) / session | 1.8 |
| Sessions reading research docs | **7 / 21 = 33%** |

## Matches (15 verified)

| Session | Address | Attempts |
|---------|---------|----------|
| 1c72153d | 0x0001218c | 1 |
| 25957895 | 0x00111c08 | 1 |
| 25957895 | 0x001110fc | 1 |
| 266244bd | 0x00103788 | 3 |
| 31119d3c | 0x001a7758 | 1 |
| 4b7556f0 | 0x000e51c8 | 7 |
| 5160fab3 | 0x00012c94 | 1 |
| 6fb12182 | 0x0006c880 | 4 |
| 76ea7d52 | 0x0006c118 | 1 |
| 76ea7d52 | 0x0006c928 | 2 |
| b4da2b68 | 0x0000b23c | 1 |
| bc92252d | 0x00011d64 | 1 |
| bc92252d | 0x00013194 | 4 |
| e1470084 | 0x0000a750 | 1 |
| 888688e8 | 0x00047aa4 | 2 (but **broke `0x00047f80` SetSkin** in same TU) |

(Two more matches — session 2d06b88e `0x00103374` after 5 attempts, and session 930ddec5 `0x000ebe48` — were claimed by agents but rejected by `post_revert_verify_failed` after orchestrator detected out-of-scope header edits and reverted.)

## Failure Analysis

### By Category (25 distinct failures from session_results)

| Category | Count | Notes |
|----------|-------|-------|
| REGISTER_ALLOC | ~14 | Dominant theme. Structure correct, instructions correct, byte diff comes from SNC's allocator picking different s-regs (s1↔s2 swaps in particular). Permuter run repeatedly with zero improvement. |
| SIZE_MISMATCH (4 / 8 bytes short) | ~5 | Compiles smaller than expected — likely missing a tail instruction the original SNC emitted (NOP padding, register move, branch-likely fill). |
| SCHEDULING / BLOCK_ORDER | ~3 | E.g. session 5160fab3 — SNC emits volatile/non-volatile branches in the opposite block order than the original. |
| LARGE_DIFF (>20% bytes) | ~3 | Closer to "wrong approach" than "last-mile" — agent stuck without converging. |
| AGENT_OUT_OF_SCOPE_EDIT | 4 sessions | Agent edited `include/gcUIDialog.h`, `include/eMemCard.h`. Reverted, two of those were post-revert compile failures. |

### Highlights

**`cFactory::MarkForClean` and `CleanGroups` (0xbbd4 / 0xb648):** TWO different sessions (499f07ac, 7a34804a) attempted these and both reported the *same* 7-byte register swap (i and group/array pointer get reversed s1↔s2 assignments). Both ran the permuter for >180s with zero improvement. These are sibling functions exhibiting the same SNC quirk — worth a focused research doc on cFactory's allocation pattern, since multiple agents are spending hours on it.

**`gcViewport::UpdateUI` (0x000fdddc, session cf7a051d):** "SAME compiler quirk as the previously failed UpdateAll at 0x000fdd44 (sibling function, identical structure documented in functions.json)." So the prior run's dead-end carried into this run unsolved.

**Two `FindPlayingCinematic` overloads (eceb75ce):** 29-byte and 58-byte diffs after permuter exhausted thousands of candidates. Both circular doubly-linked-list iteration patterns over `D_0037D7C4` — likely missing a known SNC list-iteration codegen pattern not yet captured in research.

**`gcUIDialog::Write` and `CreateControl` work (4b7556f0):** while pursuing CreateControl as the new match, the agent damaged sibling matches (`OnMemPoolReset`, `PausesGame`) — see BROKEN section above.

## Agent Behavior

- **Research docs read by only 7/21 sessions (33%)** — well below where it should be given how many failures bottomed out at "register allocation we can't influence." Agents that read `snc-fpu-scheduling.md` and `snc-vtable-layout.md` matched faster.
- **Avg 102 tool calls / 18.8 compare_func iterations / 1.8 src writes per session** — agents iterate a lot via compare but rarely write distinct restructurings. Looks like many sessions are hitting the same dead-end repeatedly with minor tweaks.
- **9 / 21 sessions had their work reverted by the ledger** — orchestrator caught out-of-scope header edits, unreported function changes, and post-revert compile failures. Guardrails worked. But ~43% session waste is a strategy problem, not a guardrail problem.
- **Two post_revert_verify_failed events** (sessions 930ddec5 and 2d06b88e) — the agent edited an out-of-scope header for the match to compile; orchestrator reverted the header; the agent's src file then no longer compiled. These claimed matches were correctly rejected, but the symptom is agents reaching for header edits to paper over missing class members rather than declaring the right struct layout in their .cpp. Investigate the prompts.

## Recommendations

1. **Resolve the 5 byte-mismatch DB entries** (the BROKEN section). For the 3 caused by tonight's edits, decide revert vs accept-and-rematch. For the 2 preexisting, do a one-time cleanup pass.
2. **Add `verify_matches.py` to `run_overnight.sh`'s pre-flight** — run it before the orchestrator starts and refuse to start if there are problems. The 2 preexisting drift entries say no one runs it routinely.
3. **Same-TU-sibling protection.** When a session edits a `.cpp` that already contains matched functions, the orchestrator should re-verify all sibling matches in that file post-session and reject the session if any regressed. The agent guardrails currently only check out-of-scope file edits, not in-scope regression.
4. **Pre-prompt cFactory + gcViewport register-allocation research.** Both classes have multiple sibling functions exhibiting the *same* unsolved SNC s1↔s2 quirk. Two separate agents in this run spent 2 hours each making identical observations. Either add this to the research corpus with a workaround, or mark these unmatchable per `docs/decisions/006`.
5. **Force research-doc reads** (only 33% of sessions read any). Either inline the research summaries into the prompt or fail prompts that don't reference the relevant doc.
6. **Lower the "header-modification" temptation** in the prompt — 4 separate sessions tried to add missing methods to header files. The repeated pattern suggests m2c's initial decompilation produces calls to nonexistent class members, and agents reach for the header rather than rewriting the call.

## Decision required

The 5 byte-mismatch entries make the DB's "matched" count untrustworthy. **Recommend running `python3 tools/verify_matches.py --fix` (or whatever the cleanup mode is) before starting the next overnight, after deciding what to do about the 3 tonight-caused regressions.**
