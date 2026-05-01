# Pre-Finetune Matching Progress

Living document tracking the 44 strategic functions from `config/finetune_targets.json` that must be matched before synthetic data generation. See `docs/decisions/004-pre-finetune-matching-targets.md` for selection rationale.

**Status after overnight run 2026-04-11:** 16 matched, 12 failed, 16 untried.

## Matched (16)

| Address | Function | Size | Category | Notes |
|---|---|---|---|---|
| 0x0004064c | ePortal::ConnectRoom | 124B | transition_zone | Confirms sched=2 at zone start |
| 0x0004e234 | eDynamicMesh::eDynamicMesh(cBase *) | 152B | transition_zone | Confirms sched=2 mid-zone |
| 0x00052bb0 | eHeightmap::CastSphere | 132B | transition_zone | Confirms sched=2 mid-zone. VFPU loads/stores for vector args |
| 0x0005f388 | eStaticSkyLight::GetDirectLight | 184B | transition_zone | Confirms sched=2 late-zone (contradicts initial expectation of sched=1) |
| 0x00065bbc | eBipedController::GetVelocity | 160B | transition_zone | Confirms sched=2 late-zone |
| 0x0006afbc | eCapsuleShape::GetSupport | 104B | transition_zone | Confirms sched=2 at zone end boundary |
| 0x0006c490 | eSimulatedController::GetVelocity | 180B | transition_zone | Confirms sched=2 at zone end |
| 0x0006ea60 | eCollisionConstraint::Allocate | 68B | sched1_verify | Confirms sched=1 for eCollisionConstraint |
| 0x0003b954 | eInputKeyboard::KeyToASCII | 128B | sched1_verify | Confirms sched=1 for eInputKeyboard |
| 0x001a19cc | mFrustum::GetOrtho | 84B | mall_verify | Confirms sched=2 for mAll_psp.obj |
| 0x001a2f38 | mQuat::Mult | 180B | mall_verify | Pure VFPU quaternion multiply. Matched via inline asm |
| 0x001a020c | mBasis::Orthonormalize | 196B | mall_verify | 100% VFPU (Gram-Schmidt). Matched via inline asm |
| 0x001edb78 | eDynamicGeom::GetSubObjectToWorld | 112B | chunk2_transition | VFPU matrix load/store |
| 0x001e8a0c | eRayCast::eRayCast | 204B | chunk2_transition | VFPU vector copy for origin/direction |
| 0x002095e0 | eMultiSphereShape::GetProjectedMinMax | 144B | chunk2_transition | VFPU dot products |
| 0x0001ce74 | eAudioFader::Update | 196B | training_loop | Float interpolation with cTimeValue |

## Failed (12) — Analysis

### VFPU-heavy failures (3)

**mVec3::MakeNormal** (0x0019f598, 68B, 94% VFPU)
- 16 VFPU instructions in 68 bytes — almost entirely vector cross product + normalize
- Agent needs to produce inline asm for every instruction with correct register assignments
- mBasis::Orthonormalize (100% VFPU, 196B) DID match, so this should be retryable
- Likely issue: small function means less scalar "skeleton" for m2c to provide, agent starts with less context

**mBox::Extend** (0x001a0a74, 132B, 63% VFPU)
- AABB min/max extension using VFPU vector min/max operations
- 21 VFPU instructions. Should be a straightforward VFPU pattern
- Retry candidate — similar complexity to matched functions

**gcCamera::gcEntityFollowState ctor** (0x00248e58, 564B, 36% VFPU)
- Large constructor with 52 VFPU instructions initializing camera state
- Mix of scalar field inits and VFPU matrix/vector setup
- Failed alongside eAudioChannel::CalcPanning in same session (0 matched, 2 failed)
- May need batch_size=1 for more time, or better VFPU pattern context

### Large scalar failures (5)

**eCollisionConstraint::Initialize** (0x0006e7bc, 676B, pure scalar)
- Largest target. Pool allocator initialization with a loop building free list
- In sched=1 zone — confirmed by Allocate matching with sched=1
- 676B is at the limit of what a single session can iterate on
- Needs batch_size=1 and possibly longer timeout

**cRedBlackTree::FindNode** (0x001e5bc4, 500B, pure scalar)
- Template-heavy red-black tree traversal with comparisons
- Complex control flow with multiple nested conditionals
- The mangled name is enormous which may cause issues with symbol matching

**eAudioChannel::CalcPanning** (0x0001d224, 508B, 8% VFPU)
- Audio panning calculation with some VFPU for vector math
- Failed in same session as gcCamera ctor (both failed — bad session?)

**cOutStream::Write** (0x000065f0, 504B, pure scalar)
- Serialization with type dispatch (likely switch statement)
- Failed alongside WriteBits in same session (both failed)
- Switch codegen at this scale may need specific SNC pattern knowledge

**gcLookAtController::SetHPR** (0x001473e8, 640B, pure scalar)
- Largest pure-scalar target. Float math + conditionals
- Heading/Pitch/Roll with trig — dense floating point

### Small/medium surprising failures (4)

**eDynamicGeom::eDynamicGeom** (0x00044c44, 140B, 25% VFPU)
- Constructor with identity matrix init via VFPU
- 9 VFPU instructions. Should be matchable — similar pattern to matched ctors
- Retry with notes about vmidt.q + vmov.q + sv.q pattern for identity matrix

**eDynamicModel::NeedsSkinning** (0x00047a08, 156B, pure scalar)
- No VFPU, only 156B — this SHOULD have matched
- Accesses mesh submesh data via pointer chains. Complex field offsets
- May need better header context (eDynamicMesh struct layout)

**ePath::PathT2Units** (0x0005bab0, 176B, pure scalar)
- Path interpolation function, no VFPU
- Likely needs ePath struct layout (points array, total length)
- Header exists but may be incomplete

**cOutStream::WriteBits** (0x000068c4, 296B, pure scalar)
- Bit-level stream writing with shift/mask loops
- Failed alongside cOutStream::Write — both in same session
- Algorithmic loop with bitwise operations — SNC may schedule these unusually

## Untried (16)

| Address | Function | Size | Category |
|---|---|---|---|
| 0x000d5348 | gcExpressionList::Evaluate | 232B | training_loop |
| 0x002fa058 | gcDoReturn::Evaluate | 192B | training_switch |
| 0x00150260 | gcValCaseRange::Evaluate | 180B | training_switch |
| 0x002f9e98 | gcDoReturn::Write | 124B | training_serial |
| 0x002f9f14 | gcDoReturn::Read | 324B | training_serial |
| 0x0003417c | eCollisionPair::BodiesSleeping | 60B | training_bool |
| 0x0011887c | gcEntity::GetSoundFrequencyOffset | 96B | training_bool |
| 0x0019fb80 | mBasis::SetQuat | 216B | training_float |
| 0x001a1544 | mSphere::CalcEmbeddedContact | 168B | training_float |
| 0x001a69e8 | nwConnection::UpdatePing | 216B | nw_verify |
| 0x0007ae64 | eTextureMap::Read | 264B | sched1_large |
| 0x0007340c | eCompoundShape::GetEmbedContacts | 400B | sched1_large |
| 0x0007359c | eCompoundShape::GetAABB | 336B | sched1_large |
| 0x0000440c | cCRC::Calc | 308B | algorithmic |
| 0x000d58c0 | gcEventParams::Write | 548B | large_function |
| 0x000731e4 | eCompoundShape::GetSweptContacts | 552B | sched1_large |

## Key Findings

1. **Transition zone is mixed, not address-only.** The original probe set showed several specific `sched=2` matches in 0x040000-0x06e000, but later matched methods and failure notes refute the broader "all sched=2" conclusion. Use the newer class/method map in `docs/research/snc-transition-zone-sched.md`.

2. **mAll_psp.obj confirmed sched=2.** mFrustum::GetOrtho matched with default flags.

3. **VFPU functions are matchable.** mBasis::Orthonormalize (100% VFPU) and mQuat::Mult both matched, proving the inline asm approach works. The m2c VFPU passthrough fork was essential.

4. **Large functions (>400B) need more time.** All 5 pure-scalar failures above 400B suggest batch_size=1 and possibly longer sessions.

5. **No retry context hurts.** Failed sessions start from scratch with no knowledge of what was tried. The 4 small/medium failures are likely solvable with notes from the first attempt.
