# Current Matching Prep

Generated: 2026-05-19

## Match Progress

The tracked progress chart for this snapshot is in [`docs/research/match-progress.md`](match-progress.md).

Regenerate this snapshot from the repo root with:

```bash
python3 tools/generate_matching_prep.py \
  --limit-high-yield 240 \
  --high-yield-out config/targets_high_yield_untried_20260519.json \
  --failed-retry-out config/targets_failed_nearmiss_20260519.json \
  --read-research-out config/targets_read_cblock_research_20260519.json \
  --tagged-research-out config/targets_tagged_pointer_research_20260519.json \
  --codegen-research-out config/targets_codegen_research_20260519.json \
  --report docs/research/current-matching-prep-20260519.md
```

The failed-retry output had zero rows for this snapshot and is not committed as a runnable target queue.

## DB Summary

| Status | Count |
|---|---:|
| `matched` | 4973 |
| `untried` | 3062 |
| `failed` | 1181 |
| `unmatchable_symbol_mangling` | 26 |

## Top Families

| Family | Matched | Failed | Untried | Observed rate |
|---|---:|---:|---:|---:|
| `VisitReferences` | 0 | 18 | 302 | 0.0% |
| `Evaluate` | 26 | 14 | 148 | 65.0% |
| `operator=` | 13 | 29 | 142 | 31.0% |
| `Read` | 448 | 178 | 100 | 71.6% |
| `Destructor` | 454 | 119 | 51 | 79.2% |
| `Update` | 13 | 9 | 42 | 59.1% |
| `SetSize` | 2 | 6 | 37 | 25.0% |
| `Reset` | 35 | 19 | 29 | 64.8% |
| `Get` | 26 | 16 | 26 | 61.9% |
| `Set` | 27 | 16 | 20 | 62.8% |
| `Apply` | 7 | 5 | 19 | 58.3% |
| `Draw` | 6 | 7 | 17 | 46.2% |
| `Constructor` | 289 | 48 | 16 | 85.8% |
| `AssignCopy` | 364 | 130 | 13 | 73.7% |
| `Collide` | 52 | 27 | 13 | 65.8% |
| `Cull` | 3 | 5 | 13 | 37.5% |
| `Replicate` | 0 | 1 | 13 | 0.0% |
| `Intersect` | 0 | 0 | 11 | n/a |
| `ApplyDynamic` | 7 | 1 | 9 | 87.5% |
| `GetEmbedContacts` | 5 | 1 | 8 | 83.3% |
| `GetSweptContacts` | 6 | 0 | 8 | 100.0% |
| `Initialize` | 16 | 5 | 8 | 76.2% |
| `mContainment)` | 0 | 0 | 8 | n/a |
| `Activate` | 3 | 1 | 7 | 75.0% |

## Failure Classifier

| Primary | Functions |
|---|---:|
| `regalloc_drift` | 513 |
| `agent_low_confidence` | 297 |
| `read_prologue_compiler` | 155 |
| `branch_direction_codegen` | 118 |
| `needs_layout` | 63 |
| `constant_cse_codegen` | 23 |
| `tagged_pointer_shape` | 8 |
| `large_semantic` | 3 |
| `retryable_nearmiss` | 1 |

| Action | Functions |
|---|---:|
| `quarantine` | 513 |
| `research` | 305 |
| `retry` | 298 |
| `prep` | 65 |

| Tag | Bytes |
|---|---:|
| `regalloc_drift` | 238632 |
| `branch_direction_codegen` | 157980 |
| `needs_layout` | 114784 |
| `read_prologue_compiler` | 78276 |
| `tagged_pointer_shape` | 59644 |
| `agent_low_confidence` | 50852 |
| `constant_cse_codegen` | 37740 |
| `large_semantic` | 27356 |
| `retryable_nearmiss` | 1764 |

## Failure Quarantine Signals

| Signal | Functions | Bytes |
|---|---:|---:|
| `read_cblock_prologue` | 156 | 79708 |
| `regalloc` | 619 | 254704 |
| `tagged_pointer_branch` | 116 | 61924 |
| `large_semantic` | 19 | 27356 |
| `shape_collide_regalloc` | 27 | 3640 |
| `tooling_or_placement` | 0 | 0 |
| `uncategorized_failed` | 0 | 0 |

## Generated Target Files

| File | Rows | Bytes | Purpose |
|---|---:|---:|---|
| `high_yield` | 240 | 204220 | Best untried non-Read families for future matching runs. |
| `failed_retry` | 0 | 0 | Small failed near-misses after compiler-blocked buckets are filtered out. |
| `read_research` | 90 | 29316 | Remaining cReadBlock prologue/scheduler research targets. |
| `tagged_research` | 40 | 16568 | Tagged/nullable-pointer branch-shape research targets. |
| `codegen_research` | 80 | 21656 | Regalloc, branch-direction, and constant-CSE compiler/codegen research targets. |

## Preview: high_yield

| Address | Size | Score | Function | Reason |
|---|---:|---:|---|---|
| `0x0003b858` | 132 | 96 | `eInputMouse::eInputState::Reset(void)` | Reset family; exact-size matched method template; manageable size |
| `0x00286094` | 288 | 74 | `gcNetworkConfigStrings::Get(wchar_t *, int) const` | Get family; manageable size |
| `0x0028b2d8` | 460 | 74 | `gcTableString::Get(cHandlePairT<gcStringTable, cSubHandleT<gcString> > *) const` | Get family; manageable size |
| `0x0028b0fc` | 476 | 74 | `gcTableString::Get(wchar_t *, int) const` | Get family; manageable size |
| `0x002fcc5c` | 708 | 73 | `gcDoSetEventObject::AssignCopy(const cBase *)` | AssignCopy family; larger target |
| `0x002fd814` | 724 | 73 | `gcDoSetEventObjectArray::AssignCopy(const cBase *)` | AssignCopy family; larger target |
| `0x0030cb78` | 756 | 73 | `gcDoUIListOp::AssignCopy(const cBase *)` | AssignCopy family; larger target |
| `0x002fc008` | 764 | 73 | `gcDoSetEventEnumParam::AssignCopy(const cBase *)` | AssignCopy family; larger target |
| `0x0031025c` | 772 | 73 | `gcDoUISetSprite::AssignCopy(const cBase *)` | AssignCopy family; larger target |
| `0x00306334` | 784 | 73 | `gcDoTableQuery::AssignCopy(const cBase *)` | AssignCopy family; larger target |
| `0x0033fcac` | 788 | 73 | `gcValEntityVariable::AssignCopy(const cBase *)` | AssignCopy family; larger target |
| `0x002c3fe4` | 792 | 73 | `gcDoEntitySetAttractor::AssignCopy(const cBase *)` | AssignCopy family; larger target |
| `0x003377ac` | 804 | 73 | `gcValEntityPartialControllerVariable::AssignCopy(const cBase *)` | AssignCopy family; larger target |
| `0x0031b9cc` | 828 | 73 | `gcGeomCurveConfig::AssignCopy(const cBase *)` | AssignCopy family; larger target |
| `0x002087a8` | 280 | 70 | `eSweptMultiSphereCollisionHandler::Set(const mCapsule &)` | Set family; manageable size |
| `0x002094c8` | 280 | 70 | `eEmbedMultiSphereCollisionHandler::Set(const mCapsule &)` | Set family; manageable size |
| `0x003639e0` | 408 | 70 | `gcValUIOpacity::Set(float)` | Set family; manageable size |
| `0x00322b50` | 424 | 70 | `gcValCameraVariable::Set(float)` | Set family; manageable size |
| `0x0035f760` | 460 | 70 | `gcValTableEntry::Set(float)` | Set family; manageable size |
| `0x0028b4a4` | 464 | 70 | `gcTableString::Set(const wchar_t *) const` | Set family; manageable size |
| ... | ... | ... | 220 more | |

## Preview: failed_retry

| Address | Size | Score | Function | Reason |
|---|---:|---:|---|---|

## Preview: read_research

| Address | Size | Score | Function | Reason |
|---|---:|---:|---|---|
| `0x00086074` | 344 | 218 | `eShadowVolumeModelMtl::Read(cFile &, cMemPool *)` | cReadBlock prologue/scheduler research; body reported close |
| `0x00081040` | 352 | 216 | `eFilteredTexture::Read(cFile &, cMemPool *)` | cReadBlock prologue/scheduler research; body reported close |
| `0x003265e0` | 296 | 211 | `gcValEntityConstant::Read(cFile &, cMemPool *)` | cReadBlock prologue/scheduler research; body reported close |
| `0x0032ec54` | 296 | 211 | `gcValEntityHasAnimation::Read(cFile &, cMemPool *)` | cReadBlock prologue/scheduler research; body reported close |
| `0x00339018` | 308 | 211 | `gcValEntityPathDirection::Read(cFile &, cMemPool *)` | cReadBlock prologue/scheduler research; body reported close |
| `0x003356e4` | 328 | 211 | `gcValEntityIsValid::Read(cFile &, cMemPool *)` | cReadBlock prologue/scheduler research; body reported close |
| `0x000815e0` | 312 | 208 | `eFrameBufferTexture::Read(cFile &, cMemPool *)` | cReadBlock prologue/scheduler research; body reported close |
| `0x0033da60` | 404 | 208 | `gcValEntitySoundFrequency::Read(cFile &, cMemPool *)` | cReadBlock prologue/scheduler research; body reported close |
| `0x00327408` | 268 | 207 | `gcValEntityControllerVariable::Read(cFile &, cMemPool *)` | cReadBlock prologue/scheduler research; body reported close |
| `0x00330bb8` | 296 | 207 | `gcValEntityHasTimeSince::Read(cFile &, cMemPool *)` | cReadBlock prologue/scheduler research; body reported close |
| `0x0033eef8` | 296 | 207 | `gcValEntityTimeSince::Read(cFile &, cMemPool *)` | cReadBlock prologue/scheduler research; body reported close |
| `0x000211d8` | 120 | 206 | `eSound::Read(cFile &, cMemPool *)` | cReadBlock prologue/scheduler research; body reported close |
| `0x00008ea4` | 172 | 206 | `cNamed::Read(cFile &, cMemPool *)` | cReadBlock prologue/scheduler research; body reported close |
| `0x0003c4f8` | 228 | 206 | `eFluidVolume::Read(cFile &, cMemPool *)` | cReadBlock prologue/scheduler research; body reported close |
| `0x0002b820` | 268 | 206 | `eShape::Read(cFile &, cMemPool *)` | cReadBlock prologue/scheduler research; body reported close |
| `0x001a4d9c` | 336 | 206 | `nwConfigBase::Read(cFile &, cMemPool *)` | cReadBlock prologue/scheduler research; body reported close |
| `0x00125d3c` | 232 | 204 | `gcEntitySoundConfig::Read(cFile &, cMemPool *)` | cReadBlock prologue/scheduler research; body reported close |
| `0x0002d044` | 260 | 204 | `eCameraEffectLayer::Read(cFile &, cMemPool *)` | cReadBlock prologue/scheduler research; body reported close |
| `0x002a9a14` | 1052 | 149 | `gcDoEntityCastRay::Read(cFile &, cMemPool *)` | cReadBlock prologue/scheduler research; body reported close |
| `0x000e8e8c` | 224 | 200 | `gcAnimationEvent::Read(cFile &, cMemPool *)` | cReadBlock prologue/scheduler research; body reported close |
| ... | ... | ... | 70 more | |

## Preview: tagged_research

| Address | Size | Score | Function | Reason |
|---|---:|---:|---|---|
| `0x00136f68` | 508 | 247 | `gcStaticInstance::Read(cFile &, cMemPool *)` | tagged/nullable-pointer branch-shape research candidate |
| `0x0030df90` | 452 | 200 | `gcDoUISendMessage::Read(cFile &, cMemPool *)` | tagged/nullable-pointer branch-shape research candidate |
| `0x0014f48c` | 464 | 200 | `gcDoSetValue::Read(cFile &, cMemPool *)` | tagged/nullable-pointer branch-shape research candidate |
| `0x0015c9bc` | 344 | 200 | `gcValUnaryOp::Read(cFile &, cMemPool *)` | tagged/nullable-pointer branch-shape research candidate |
| `0x0034b384` | 532 | 200 | `gcValLobbyScoreboardInfo::Read(cFile &, cMemPool *)` | tagged/nullable-pointer branch-shape research candidate |
| `0x00350b88` | 784 | 186 | `gcValNavMeshDirection::AssignCopy(const cBase *)` | tagged/nullable-pointer branch-shape research candidate |
| `0x00343260` | 116 | 182 | `gcValExternalVariable::Set(float)` | tagged/nullable-pointer branch-shape research candidate |
| `0x002fc87c` | 244 | 174 | `gcDoSetEventEnumParam::GetText(char *) const` | tagged/nullable-pointer branch-shape research candidate |
| `0x003268c4` | 288 | 174 | `gcValEntityConstant::GetText(char *) const` | tagged/nullable-pointer branch-shape research candidate |
| `0x0032f100` | 288 | 174 | `gcValEntityHasAnimation::GetText(char *) const` | tagged/nullable-pointer branch-shape research candidate |
| `0x00330eac` | 288 | 174 | `gcValEntityHasTimeSince::GetText(char *) const` | tagged/nullable-pointer branch-shape research candidate |
| `0x0033f480` | 288 | 174 | `gcValEntityTimeSince::GetText(char *) const` | tagged/nullable-pointer branch-shape research candidate |
| `0x003407ec` | 288 | 174 | `gcValEntityVariable::GetText(char *) const` | tagged/nullable-pointer branch-shape research candidate |
| `0x00020168` | 496 | 173 | `eWorld::UpdateVolumeLocation(eVolume *)` | tagged/nullable-pointer branch-shape research candidate |
| `0x001503d0` | 348 | 170 | `gcDoSwitch::Read(cFile &, cMemPool *)` | tagged/nullable-pointer branch-shape research candidate |
| `0x00336f78` | 296 | 168 | `gcValLookAtControllerVariable::GetText(char *) const` | tagged/nullable-pointer branch-shape research candidate |
| `0x00346654` | 312 | 167 | `gcValHasPartialController::GetText(char *) const` | tagged/nullable-pointer branch-shape research candidate |
| `0x0030ac54` | 560 | 165 | `gcDoUIFade::Read(cFile &, cMemPool *)` | tagged/nullable-pointer branch-shape research candidate |
| `0x002a13e0` | 292 | 164 | `gcDoEntityActivatePartialController::GetText(char *) const` | tagged/nullable-pointer branch-shape research candidate |
| `0x00302b48` | 348 | 164 | `gcDoStateParentHandler::Read(cFile &, cMemPool *)` | tagged/nullable-pointer branch-shape research candidate |
| ... | ... | ... | 20 more | |

## Preview: codegen_research

| Address | Size | Score | Function | Reason |
|---|---:|---:|---|---|
| `0x00344524` | 108 | 263 | `gcValFPSInfo::Evaluate(void) const` | regalloc_drift research candidate; Register naming/live-range drift after structure is correct. |
| `0x00065b54` | 104 | 260 | `eBipedController::GetVelocity(int, const mVec3 &, mVec3 *) const` | regalloc_drift research candidate; Register naming/live-range drift after structure is correct. |
| `0x0004a284` | 176 | 259 | `eDynamicModel::SetShapeController(eRigidBodyState *, ePhysicsController *, unsigned int)` | regalloc_drift research candidate; Register naming/live-range drift after structure is correct. |
| `0x00264540` | 156 | 257 | `gcMsgPlayCinematic::New(nwMsgBuffer &) static` | regalloc_drift research candidate; Register naming/live-range drift after structure is correct. |
| `0x0008974c` | 440 | 255 | `eStandardParticleSystemMtl::~eStandardParticleSystemMtl(void)` | regalloc_drift research candidate; Register naming/live-range drift after structure is correct. |
| `0x00084444` | 452 | 255 | `eReflectionModelMtl::~eReflectionModelMtl(void)` | regalloc_drift research candidate; Register naming/live-range drift after structure is correct. |
| `0x000855e4` | 452 | 255 | `eRefractionModelMtl::~eRefractionModelMtl(void)` | regalloc_drift research candidate; Register naming/live-range drift after structure is correct. |
| `0x00047be8` | 124 | 254 | `eDynamicModel::GetSkinIndex(void) const` | regalloc_drift research candidate; Register naming/live-range drift after structure is correct. |
| `0x0007fa0c` | 60 | 252 | `eShadowFillModelMtl::Unapply(void) const` | regalloc_drift research candidate; Register naming/live-range drift after structure is correct. |
| `0x001e1134` | 52 | 243 | `eTexture::GetFullTexCoords(mVec4 *) const` | branch_direction_codegen research candidate; Branch-likely or jump-in/skip-over codegen divergence. |
| `0x00141224` | 108 | 242 | `gcBipedController::gcBipedController(cBase *)` | branch_direction_codegen research candidate; Branch-likely or jump-in/skip-over codegen divergence. |
| `0x0001d6f0` | 216 | 240 | `eAudioGroup::StopSound(cHandleT<eSoundData>, cTimeValue)` | regalloc_drift research candidate; Register naming/live-range drift after structure is correct. |
| `0x0027c258` | 108 | 238 | `gcFormatString::AssignCopy(const cBase *)` | regalloc_drift research candidate; Register naming/live-range drift after structure is correct. |
| `0x00258ff0` | 156 | 238 | `gcState::AssignCopy(const cBase *)` | regalloc_drift research candidate; Register naming/live-range drift after structure is correct. |
| `0x000569a0` | 124 | 237 | `eMemCard::CardPresent(void) static` | regalloc_drift research candidate; Register naming/live-range drift after structure is correct. |
| `0x0000c0d8` | 180 | 237 | `cFactory::OnNameChanged(void)` | regalloc_drift research candidate; Register naming/live-range drift after structure is correct. |
| `0x00004540` | 336 | 237 | `cMemPool::cMemPool(cMemAllocator *, const char *)` | regalloc_drift research candidate; Register naming/live-range drift after structure is correct. |
| `0x00110030` | 112 | 236 | `gcEntityControllerTemplate::FindAnimationSet(cHandleT<gcEnumeration>) const` | regalloc_drift research candidate; Register naming/live-range drift after structure is correct. |
| `0x001101a8` | 112 | 236 | `gcEntityControllerTemplate::FindAttackSet(cHandleT<gcEnumeration>) const` | regalloc_drift research candidate; Register naming/live-range drift after structure is correct. |
| `0x000fbe00` | 136 | 236 | `gcCamera::gcXYZMotion::Set(const mVec3 &, cTimeValue)` | regalloc_drift research candidate; Register naming/live-range drift after structure is correct. |
| ... | ... | ... | 60 more | |

## Readiness Notes

- Do not rerun stale Read follow-up target files. The 188B family is drained.
- Use the high-yield list for ordinary matching only after current research/doc updates land.
- Use the read and tagged lists as research queues, not broad overnight queues.
- Treat `read_cblock_prologue`, `regalloc`, and `shape_collide_regalloc` as quarantine signals unless a targeted compiler/source-shape project is active.
