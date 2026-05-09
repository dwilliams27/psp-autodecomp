# Read Follow-Up Failure Corpus - 2026-05-09

This note analyzes the remaining failures from the drained Read follow-up
target file:

- Run log: `logs/match_20260508_185952.jsonl`
- Failure snapshots: `logs/failure_snapshots/match_20260508_185952/`
- Target file: `config/targets_read_followup_20260508_gpt55_high_8h.json`

The run matched 69 more functions and drained the target file. The remaining
170 failures are no longer a good broad matching pool; they are a set of
compiler-shape, branch-shape, source-placement, and large reconstruction
problems.

2026-05-09 update: subsequent focused compiler-hook work matched
`eLensFlare::Read` and `gcTimer::Read`, reducing this family to 164 remaining
`Read(cFile &, cMemPool *)` failures outside `PlatformRead`. The safe hook
addition was deliberately narrow (`li a2,3` and `sw ra,40(sp)` for the
unsigned constructor). A broader signed-constructor transform was tested and
rejected after it worsened `gcValEntityConstant::Read`.

## Run Result

| Metric | Value |
| --- | ---: |
| Attempted | 239 |
| Matched | 69 |
| Failed | 170 |
| Matched bytes | 26,448 |
| Failed target bytes | 90,892 |
| Final DB matched | 4,790 / 9,010 |
| Final DB progress | 53.2% |

The full `Read(cFile &, cMemPool *)` follow-up file is now exhausted:

| Status | Count | Bytes |
| --- | ---: | ---: |
| Matched | 256 | 76,512 |
| Failed | 170 | 90,892 |
| Untried | 0 | 0 |

Do not rerun this target file. Future work should use the failure buckets below
or move to a new family.

## Failure Buckets

The exact buckets overlap, because many notes mention cReadBlock while also
having register-allocation or branch-shape drift. A useful heuristic split is:

| Bucket | Functions | Bytes | Recommended action |
| --- | ---: | ---: | --- |
| cReadBlock prologue-only / scheduler blocker | ~55-61 | ~18-19KB | Wait for compiler/scheduler work. |
| Branch-likely / tagged-pointer shaping | ~34 | ~15KB | Source-pattern research; small hand-picked retries. |
| Small near-miss, uncategorized | ~23 | ~8KB | Review individually; some are good manual targets. |
| Register allocation / TU-context drift | ~20 | ~9KB | Mostly system/compiler or original-context work. |
| Large semantic / register-pressure miss | ~18 | ~24KB | Treat as project tickets, not overnight matching. |
| Tooling / prompt / source-placement issue | ~11 | ~11KB | Fix tools/prompts before retrying. |
| Other | ~9 | ~5KB | Case-by-case. |

Strong signal from the corpus:

- 132 / 170 failure snapshots mention `cReadBlock` plus prologue/scheduler
  language somewhere in the note.
- The strong prologue-only bucket is about 61 functions, only about 1.3KB of
  residual differing bytes across roughly 17.6KB of target code.
- A broader low-diff cReadBlock bucket is about 85 functions, roughly 26KB of
  target code. Most have already had normal source levers tried.

## cReadBlock Prologue Blocker

This is still the dominant blocker. The repeated pattern:

1. The source reconstructs the body correctly.
2. The body matches after the `cReadBlock` constructor call.
3. The remaining diff is save/move/result-constant scheduling around the
   constructor, usually involving `li s3,1` or equivalent result materialization.
4. Attempts such as moving result initialization before construction, raw
   `rb[5]` ctor/dtor calls, register bindings, sched pragmas, barriers, split
   TUs, and small permuter runs usually make the function worse or unchanged.

Representative blocked rows:

- `eShape::Read` - 14/268
- `eCameraEffectLayer::Read` - 16/260
- `gcConfiguration::Read` - 20/364
- `gcBipedControllerTemplate::Read` - 21/252
- `gcDoUIEffect::Read` - 21/268
- `gcDoUISetFocus::Read` - 21/260
- `gcUIControlString::Read` - 21/240
- `gcTimer::Read` - 22/432
- `gcValEntityVariable::Read` - 23/296
- `gcValTableInfo::Read` - 32/356
- `gcValControllerState::Read` - 32/368
- `eStaticMesh::Read` - 32/360

Recommendation: do not spend more broad source-retry time here. These rows are
the best argument for continuing compiler/scheduler/LRA work around the
Read-prologue hook.

## Best Source-Level Retry Candidates

These are not guaranteed matches, but they are the best manual targets because
the remaining issue is narrow and concrete.

| Function | Size | Failure shape | Why it is interesting |
| --- | ---: | --- | --- |
| `gcDoEntityMoveToPoint::Read` | 712 | 6-byte drift | One bool-read/version-load scheduling sequence. |
| `gcValEntitySoundFrequency::Read` | 404 | 12-byte drift | Versioned bool tail; prologue/body mostly aligned. |
| `gcStaticInstance::Read` | 508 | 23-byte drift | Handle validation branch shape looks source-shapeable. |
| `gcDoUISendMessage::Read` | 452 | emits 448B | Tagged-pointer fallback branch compaction. |
| `gcValLobbyScoreboardInfo::Read` | 532 | emits 528B | Same nullable-pointer repair pattern appears twice. |
| `gcValUnaryOp::Read` | 344 | emits 340B | Two-stage `(child & 1)` materialization is too compact. |
| `eMesh::Read` | 368 | emits 364B | Version `>=4` test collapses; may benefit from isolation. |
| `gcDoSetValue::Read` | 464 | emits 452B | Two tagged-pointer fallback stores; reusable idiom target. |

These should be attacked as pattern research, not by ordinary target-list
matching. In particular, `gcDoUISendMessage`, `gcValLobbyScoreboardInfo`, and
`gcDoSetValue` likely share a nullable-pointer/ReadBase repair idiom.

## Large / Project-Sized Failures

These have valuable semantic scaffolds now, but they are not last-mile tasks.

| Function | Size | Current state | Recommendation |
| --- | ---: | --- | --- |
| `gcGame::Read` | 2940 | No real close source attempt | Split-TU/project ticket. |
| `eAnimation::Read` | 2764 | Emits 2624B | Needs cArray resize macro and register-role work. |
| `gcUITextControl::Read` | 2120 | Size-exact, 1612B diff | UI base project; legacy wchar/float arrays. |
| `gcGameSettings::Read` | 2084 | Emits 1532B | Needs real inline template/destructor behavior. |
| `eMeshShape::Read` | 1600 | Emits 1480B | VFPU/triangle loop and frame/FPU save set. |
| `gcDoInputOp::Read` | 1588 | Emits 652B | Current helper factoring is far too small. |
| `gcEntity::Read` | 1456 | Size-exact, 985B diff | cArray old path and callee-save roles diverge. |
| `eDynamicMesh::Read` | 1432 | 1004B diff | Stack-frame/local layout project. |
| `eParticleSystemTemplate::Read` | 1416 | Emits 1408B | Good large near-miss: arg scheduling and one optional branch. |
| `eRoom::Read` | 1336 | Size-exact, 973B diff | Semantic scaffold; needs register-pressure rewrite. |

Highest-value large near-matches:

- `gcCinematicEvent::Read` - 800B, only 8 bytes off, but likely register/LRA.
- `gcDoEntityCastRay::Read` - 1052B, 16-byte late scheduling/register drift.
- `gcDoEntitySpawn::Read` - 1012B, body-matched; prologue drift.
- `gcDoUIShowDialog::Read` - 1260B, 4-byte size mismatch in tagged-pointer
  name-fixup branch shapes.
- `eConfigBase::Read` - 984B, 73-byte diff with matching frame/prologue.
- `gcString::Read` - 1088B, short by 36B; legacy resize/copy path.

These are worth focused projects, but not another blind overnight run.

## Tooling / Prompt Improvements

The run exposed several non-matching sources of waste.

### Allowed Paths Per Target

One explicit `out_of_scope_path` happened because a mixed-class batch allowed
the nominal session class path but not every target's canonical path. The agent
matched `eSound::Read` by editing `src/eStaticSurfaceLight.cpp`, but the ledger
reverted it.

Fix: compute allowed write paths per target, not only per session class.

### Placement Policy

Several prompts had a suggested write target that conflicted with the owning
class. Agents then had to choose between "right class file" and "prompt write
target".

Prompt/tool metadata should expose:

- `owning_class`
- `suggested_write_target`
- `canonical_write_target`
- `placement_conflict`

Preferred placement rule:

1. Use the owning class canonical `.cpp` if safe.
2. Use a `Class_Read.cpp` split TU if header/class perturbation is risky.
3. Use the suggested write target only if it agrees with the owning class or an
   existing split-TU pattern.
4. If it conflicts, report `PLACEMENT_CONFLICT` rather than guessing.

### Permuter Isolation

`tools/permuter.py` still fails to locate target symbols in multi-function
objects. `compare_func.py` already uses better byte/symbol extraction.

Fix: make `permuter.py` use `symbols_with_bytes_and_relocs()` from
`byte_match.py`, accept/pass an explicit `--symbol`, and print candidate symbols
when isolation fails.

### Permuter Suitability Gate

Do not permute giant semantic scaffolds or wrong-stack-frame bodies.

Suggested gate:

- symbol size matches expected, or size delta <= 16 bytes;
- byte diff <= `max(128, 20% of function size)`;
- no structural blockers such as wrong stack frame, wrong placement, or missing
  symbol isolation.

Otherwise report `PERMUTER_SKIPPED_NOT_LAST_MILE`.

### Compare Tools Should Be Read-Only By Default

Agents repeatedly spent time noticing and explaining `config/functions.json`
side effects from `compare_func.py`.

Recommendation: make `compare_func.py` read-only by default and require
`--update-db` for DB mutation. The orchestrator should own DB status updates.

## Recommended Next Work

1. Implement the tooling fixes above, especially allowed-paths-per-target and
   permuter symbol isolation.
2. Continue compiler/scheduler work for cReadBlock prologue near-misses, but
   keep signed-constructor and larger-frame shapes separated from the already
   validated unsigned hook.
3. Start a small branch-pattern research project around tagged-pointer /
   nullable-pointer repair codegen. Use `gcDoUISendMessage`,
   `gcValLobbyScoreboardInfo`, and `gcDoSetValue` as the first exemplars.
4. For matching runs, move away from this drained Read target file. Build a new
   target list from non-Read near-misses or a different high-similarity family.
