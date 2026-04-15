# Known Bugs

Bugs discovered during development that haven't been fixed yet. Fix these before they bite.

## 1. Orchestrator can mark verified matches as "failed" on retry

**Found:** 2026-04-11 overnight run

When a function is matched and verified in session A, then the same source file is modified by session B (which targets a different function in the same file), session B can overwrite the source, breaking the previously matched function. If session B reports that function as "failed", the database is updated to "failed" even though a verified match existed in an earlier commit.

**Impact:** 3 functions (eAudioChannel::CalcPanning, cRedBlackTree::FindNode, gcCamera::gcEntityFollowState) were verified byte-exact against EBOOT but show as "failed" in functions.json.

**Root cause:** The orchestrator doesn't protect previously-matched functions when a later session writes to the same source file.

**Possible fixes:**
- Before marking a function as "failed", check if it was previously "matched" and re-verify against EBOOT before downgrading
- Use separate source files per function (already mostly done, but some agents put multiple functions in one file)
- Lock matched source files from further modification

## 2. ~20 matched source files are pure assembly with zero C/C++ training data

**Found:** 2026-04-14 pre-commit review

Multiple agents have submitted file-scope `__asm__()` blocks as "matches" — wrapping raw disassembly in `.cpp` files. These produce zero training data value and violate the "C/C++ source only" norm. The orchestrator now rejects new submissions like this, but existing ones remain.

**Affected files:**
- `src/eCompoundShape.cpp` (295 lines, 2 functions)
- `src/cRedBlackTree_FindNode_eCollisionPair.cpp` (187 lines)
- `src/gcEntityFollowState_ctor.cpp` (174 lines)
- `src/eAudioChannel.cpp` (155 lines)
- `src/mQuat.cpp` (136 lines, mQuat::Mult + mBasis::Orthonormalize)
- `src/eCompoundShape_GetEmbedContacts.cpp` (115 lines)
- `src/mBox.cpp` (75 lines)
- `src/mBasis_Orthonormalize.cpp` (75 lines, duplicate of symbol in mQuat.cpp)
- `src/eStaticSkyLight_GetDirectLight.cpp` (67 lines)
- `src/eSimulatedController_GetVelocity.cpp` (64 lines)
- `src/eBipedController_GetVelocity.cpp` (59 lines)
- `src/eMultiSphereShape_GetProjectedMinMax.cpp` (54 lines)
- `src/eCapsuleShape.cpp` (41 lines)
- `src/eHeightmapShape_GetInertialTensor.cpp` (15 lines)
- `src/eHeightmapShape_GetAABB.cpp` (14 lines)

**Impact:** These files are counted as "matched" (inflating the 709 count) but provide zero value for fine-tuning. Some may be reworkable as C++ with minimal inline asm; others (dense VFPU) should be re-marked as `failed`.

**Fix:** Each file needs human triage — determine if it's C++-expressible (rework) or genuinely VFPU-only (mark `failed`). The duplicate mBasis::Orthonormalize in mQuat.cpp will cause link errors if both files are included.

