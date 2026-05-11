# Current Matching Prep

Generated: 2026-05-11

## Match Progress

The tracked progress chart for this snapshot is in [`docs/research/match-progress.md`](match-progress.md).

Regenerate with `python3 tools/generate_matching_prep.py` from the repo root. This uses the current `config/functions.json` snapshot and default limits: high-yield 160, failed-retry 80, read-research 90, tagged-research 40, codegen-research 80.

## DB Summary

| Status | Count |
|---|---:|
| `matched` | 4925 |
| `untried` | 3162 |
| `failed` | 1129 |
| `unmatchable_symbol_mangling` | 26 |

## Top Families

| Family | Matched | Failed | Untried | Observed rate |
|---|---:|---:|---:|---:|
| `VisitReferences` | 0 | 18 | 302 | 0.0% |
| `Evaluate` | 26 | 14 | 148 | 65.0% |
| `operator=` | 13 | 29 | 142 | 31.0% |
| `Read` | 447 | 179 | 100 | 71.4% |
| `Destructor` | 436 | 98 | 90 | 81.6% |
| `Update` | 12 | 8 | 44 | 60.0% |
| `Reset` | 32 | 12 | 39 | 72.7% |
| `Get` | 22 | 8 | 38 | 73.3% |
| `SetSize` | 2 | 6 | 37 | 25.0% |
| `AssignCopy` | 362 | 122 | 23 | 74.8% |
| `Apply` | 7 | 2 | 22 | 77.8% |
| `Set` | 27 | 14 | 22 | 65.9% |
| `Draw` | 5 | 5 | 20 | 50.0% |
| `Cull` | 3 | 2 | 16 | 60.0% |
| `Write` | 599 | 70 | 16 | 89.5% |
| `Collide` | 52 | 27 | 13 | 65.8% |
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
| `regalloc_drift` | 445 |
| `agent_low_confidence` | 293 |
| `read_prologue_compiler` | 154 |
| `branch_direction_codegen` | 116 |
| `needs_layout` | 66 |
| `constant_cse_codegen` | 25 |
| `retryable_nearmiss` | 20 |
| `tagged_pointer_shape` | 8 |
| `large_semantic` | 2 |

| Action | Functions |
|---|---:|
| `quarantine` | 445 |
| `retry` | 313 |
| `research` | 304 |
| `prep` | 67 |

| Tag | Bytes |
|---|---:|
| `regalloc_drift` | 204916 |
| `branch_direction_codegen` | 133400 |
| `needs_layout` | 102504 |
| `read_prologue_compiler` | 77848 |
| `tagged_pointer_shape` | 57080 |
| `agent_low_confidence` | 48316 |
| `constant_cse_codegen` | 34012 |
| `large_semantic` | 26448 |
| `retryable_nearmiss` | 8156 |

## Failure Quarantine Signals

| Signal | Functions | Bytes |
|---|---:|---:|
| `read_cblock_prologue` | 155 | 79280 |
| `regalloc` | 552 | 220776 |
| `tagged_pointer_branch` | 111 | 58932 |
| `large_semantic` | 18 | 26448 |
| `shape_collide_regalloc` | 27 | 3640 |
| `tooling_or_placement` | 0 | 0 |
| `uncategorized_failed` | 0 | 0 |

## Generated Target Files

| File | Rows | Bytes | Purpose |
|---|---:|---:|---|
| `high_yield` | 160 | 90108 | Best untried non-Read families for future matching runs. |
| `failed_retry` | 18 | 4168 | Small failed near-misses after compiler-blocked buckets are filtered out. |
| `read_research` | 90 | 29316 | Remaining cReadBlock prologue/scheduler research targets. |
| `tagged_research` | 40 | 16568 | Tagged/nullable-pointer branch-shape research targets. |
| `codegen_research` | 80 | 18552 | Regalloc, branch-direction, and constant-CSE compiler/codegen research targets. |

## Preview: high_yield

| Address | Size | Score | Function | Reason |
|---|---:|---:|---|---|
| `0x000fd6ac` | 596 | 95 | `gcCamera::gcState::Write(cOutStream &) const` | Write family |
| `0x001359dc` | 612 | 95 | `gcMsgAckEntityState::Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const` | Write family |
| `0x00113acc` | 644 | 95 | `gcEntity::Write(cFile &) const` | Write family |
| `0x00134c90` | 664 | 95 | `gcMsgUpdateEntityState::Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const` | Write family |
| `0x000700fc` | 676 | 95 | `eConvexHullShape::Write(cFile &) const` | Write family |
| `0x000e29f4` | 728 | 87 | `gcUIDialog::Write(cFile &) const` | Write family; 1 matched same-class method(s); larger target |
| `0x0020adf4` | 560 | 85 | `eConvexHullShape::AssignCopy(const cBase *)` | AssignCopy family |
| `0x0026a160` | 576 | 85 | `gcDesiredEventParams::AssignCopy(const cBase *)` | AssignCopy family |
| `0x000b6420` | 712 | 83 | `eParticleSystemTemplate::Write(cFile &) const` | Write family; larger target |
| `0x0003d2e8` | 748 | 83 | `eRoom::Write(cFile &) const` | Write family; larger target |
| `0x0010b764` | 764 | 83 | `gcStateInfo::Write(cOutStream &) const` | Write family; larger target |
| `0x001a5bc4` | 768 | 83 | `nwMsgVoiceUpdateStatus::Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const` | Write family; larger target |
| `0x000fca74` | 780 | 83 | `gcCamera::gcEntityFollowState::Write(cOutStream &) const` | Write family; larger target |
| `0x0001da88` | 836 | 83 | `eAudio::Write(cOutStream &) static` | Write family; larger target |
| `0x00125ff0` | 872 | 83 | `gcEntityTemplate::Write(cFile &) const` | Write family; larger target |
| `0x0004eac0` | 876 | 83 | `eMeshShape::Write(cFile &) const` | Write family; larger target |
| `0x000794a0` | 292 | 80 | `eGeomTrail::~eGeomTrail(void)` | Destructor family; manageable size |
| `0x002df88c` | 320 | 80 | `gcDoLoadRegionSet::~gcDoLoadRegionSet(void)` | Destructor family; manageable size |
| `0x00130a8c` | 324 | 80 | `gcUIWidgetList::~gcUIWidgetList(void)` | Destructor family; manageable size |
| `0x00052168` | 348 | 80 | `eHeightmapTemplate::~eHeightmapTemplate(void)` | Destructor family; manageable size |
| ... | ... | ... | 140 more | |

## Preview: failed_retry

| Address | Size | Score | Function | Reason |
|---|---:|---:|---|---|
| `0x000d8654` | 180 | 202 | `gcUIWidget::Reset(cMemPool *, bool)` | near-miss 8B; Reset retry family; has failure snapshot; manageable size |
| `0x0000c0d8` | 180 | 187 | `cFactory::OnNameChanged(void)` | near-miss 3B; has failure snapshot; manageable size |
| `0x00244064` | 292 | 187 | `gcStreamedCinematic::AssignCopy(const cBase *)` | near-miss 3B; AssignCopy retry family; manageable size |
| `0x00075100` | 40 | 183 | `eKeyframedController::GetInvMass(int, float *, mVec3 *) const` | near-miss 7B; has failure snapshot; manageable size |
| `0x00048cc8` | 180 | 173 | `eDynamicModel::ResetAnimationState(void)` | near-miss 17B; has failure snapshot; manageable size |
| `0x001f4d9c` | 36 | 172 | `eHeightmapShape::GetInertialTensor(float, mVec3 *) const` | near-miss 18B; has failure snapshot; manageable size |
| `0x0002c154` | 128 | 168 | `eMaterial::operator=(const eMaterial &)` | near-miss 2B; manageable size |
| `0x0006c5d4` | 120 | 165 | `eSimulatedController::GetInvMass(int, float *, mVec3 *) const` | near-miss 25B; has failure snapshot; manageable size |
| `0x0006ddf8` | 96 | 164 | `eBoxShape::GetInertialTensor(float, mVec3 *) const` | near-miss 6B; manageable size |
| `0x00044c44` | 140 | 162 | `eDynamicGeom::eDynamicGeom(cBase *)` | near-miss 8B; manageable size |
| `0x001eea5c` | 512 | 158 | `cArrayBase<cArrayBase> & cArrayBase<unsigned int>::operator=(const cArrayBase<cArrayBase> &)` | near-miss 12B; manageable size |
| `0x0004269c` | 128 | 158 | `eStaticModel::CastSphere(const eCollisionInfo &, const mRay &, float, mCollideHit *) const` | near-miss 32B; has failure snapshot; manageable size |
| `0x0024bc50` | 512 | 154 | `cArrayBase<cArrayBase> & cArrayBase<cHandleT<gcTableTemplate> >::operator=(const cArrayBase<cArrayBase> &)` | near-miss 16B; manageable size |
| `0x00241684` | 512 | 146 | `cArrayBase<cArrayBase> & cArrayBase<cHandleT<gcEntityCustomAnimation> >::operator=(const cArrayBase<cArrayBase> &)` | near-miss 24B; manageable size |
| `0x00268344` | 512 | 146 | `cArrayBase<cArrayBase> & cArrayBase<cHandleT<gcEnumeration> >::operator=(const cArrayBase<cArrayBase> &)` | near-miss 24B; manageable size |
| `0x00002ec8` | 132 | 146 | `cFastMemAllocator::Reset(void)` | near-miss 64B; Reset retry family; has failure snapshot; manageable size |
| `0x000e4e34` | 352 | 140 | `gcUIDialog::Draw(unsigned int)` | near-miss 50B; has failure snapshot; manageable size |
| `0x00004b38` | 116 | 127 | `cStr::cStr(const char *, ...)` | near-miss 43B; manageable size |

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
| `0x00264540` | 156 | 257 | `gcMsgPlayCinematic::New(nwMsgBuffer &) static` | regalloc_drift research candidate; Register naming/live-range drift after structure is correct. |
| `0x00047be8` | 124 | 254 | `eDynamicModel::GetSkinIndex(void) const` | regalloc_drift research candidate; Register naming/live-range drift after structure is correct. |
| `0x0007fa0c` | 60 | 252 | `eShadowFillModelMtl::Unapply(void) const` | regalloc_drift research candidate; Register naming/live-range drift after structure is correct. |
| `0x001e1134` | 52 | 242 | `eTexture::GetFullTexCoords(mVec4 *) const` | branch_direction_codegen research candidate; Branch-likely or jump-in/skip-over codegen divergence. |
| `0x0001d6f0` | 216 | 240 | `eAudioGroup::StopSound(cHandleT<eSoundData>, cTimeValue)` | regalloc_drift research candidate; Register naming/live-range drift after structure is correct. |
| `0x00008cf4` | 152 | 240 | `cName::Write(cOutStream &) const` | branch_direction_codegen research candidate; Branch-likely or jump-in/skip-over codegen divergence. |
| `0x0004a284` | 176 | 240 | `eDynamicModel::SetShapeController(eRigidBodyState *, ePhysicsController *, unsigned int)` | branch_direction_codegen research candidate; Branch-likely or jump-in/skip-over codegen divergence. |
| `0x0027c258` | 108 | 238 | `gcFormatString::AssignCopy(const cBase *)` | regalloc_drift research candidate; Register naming/live-range drift after structure is correct. |
| `0x00258ff0` | 156 | 238 | `gcState::AssignCopy(const cBase *)` | regalloc_drift research candidate; Register naming/live-range drift after structure is correct. |
| `0x000569a0` | 124 | 237 | `eMemCard::CardPresent(void) static` | regalloc_drift research candidate; Register naming/live-range drift after structure is correct. |
| `0x00110030` | 112 | 236 | `gcEntityControllerTemplate::FindAnimationSet(cHandleT<gcEnumeration>) const` | regalloc_drift research candidate; Register naming/live-range drift after structure is correct. |
| `0x000fbe00` | 136 | 236 | `gcCamera::gcXYZMotion::Set(const mVec3 &, cTimeValue)` | regalloc_drift research candidate; Register naming/live-range drift after structure is correct. |
| `0x0008d63c` | 272 | 236 | `eRenderSurface::Initialize(int, int, unsigned int, bool, int)` | regalloc_drift research candidate; Register naming/live-range drift after structure is correct. |
| `0x0023a890` | 156 | 235 | `gcString::AssignCopy(const cBase *)` | regalloc_drift research candidate; Register naming/live-range drift after structure is correct. |
| `0x00210e5c` | 416 | 235 | `eGeomCurve::AssignCopy(const cBase *)` | regalloc_drift research candidate; Register naming/live-range drift after structure is correct. |
| `0x00065454` | 440 | 235 | `eBipedController::PostUpdate(mOCS *, eDynamicModel *) const` | regalloc_drift research candidate; Register naming/live-range drift after structure is correct. |
| `0x002a54fc` | 132 | 234 | `gcPartialBodyControllerTemplate::AssignCopy(const cBase *)` | regalloc_drift research candidate; Register naming/live-range drift after structure is correct. |
| `0x0011f550` | 100 | 232 | `gcPlayer::GetPlayerForCamera(const gcCamera *) static` | regalloc_drift research candidate; Register naming/live-range drift after structure is correct. |
| `0x00005458` | 136 | 232 | `cWriteBlock::cWriteBlock(cFile &, unsigned int)` | regalloc_drift research candidate; Register naming/live-range drift after structure is correct. |
| ... | ... | ... | 60 more | |

## Readiness Notes

- Do not rerun stale Read follow-up target files. The 188B family is drained.
- Use the high-yield list for ordinary matching only after current research/doc updates land.
- Use the read and tagged lists as research queues, not broad overnight queues.
- Treat `read_cblock_prologue`, `regalloc`, and `shape_collide_regalloc` as quarantine signals unless a targeted compiler/source-shape project is active.
