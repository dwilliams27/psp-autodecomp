# Current Matching Prep

Generated: 2026-05-09

## Match Progress

The tracked progress chart for this snapshot is in [`docs/research/match-progress.md`](match-progress.md).

## DB Summary

| Status | Count |
|---|---:|
| `matched` | 4796 |
| `untried` | 3327 |
| `failed` | 1093 |
| `unmatchable_symbol_mangling` | 26 |

## Top Families

| Family | Matched | Failed | Untried | Observed rate |
|---|---:|---:|---:|---:|
| `VisitReferences` | 0 | 18 | 302 | 0.0% |
| `Destructor` | 328 | 84 | 212 | 79.6% |
| `Evaluate` | 26 | 14 | 148 | 65.0% |
| `operator=` | 13 | 29 | 142 | 31.0% |
| `Read` | 447 | 179 | 100 | 71.4% |
| `Get` | 19 | 4 | 45 | 82.6% |
| `Update` | 12 | 8 | 44 | 60.0% |
| `Reset` | 30 | 12 | 41 | 71.4% |
| `SetSize` | 2 | 6 | 37 | 25.0% |
| `AssignCopy` | 362 | 115 | 30 | 75.9% |
| `Draw` | 4 | 0 | 26 | 100.0% |
| `Apply` | 7 | 0 | 24 | 100.0% |
| `Set` | 27 | 12 | 24 | 69.2% |
| `Cull` | 2 | 0 | 19 | 100.0% |
| `Write` | 599 | 70 | 16 | 89.5% |
| `GetInstanceType` | 13 | 0 | 14 | 100.0% |
| `Collide` | 52 | 27 | 13 | 65.8% |
| `Replicate` | 0 | 1 | 13 | 0.0% |
| `Intersect` | 0 | 0 | 11 | n/a |
| `ApplyDynamic` | 7 | 1 | 9 | 87.5% |
| `GetEmbedContacts` | 5 | 1 | 8 | 83.3% |
| `GetSweptContacts` | 6 | 0 | 8 | 100.0% |
| `Initialize` | 16 | 5 | 8 | 76.2% |
| `mContainment)` | 0 | 0 | 8 | n/a |

## Failure Quarantine Signals

| Signal | Functions | Bytes |
|---|---:|---:|
| `read_cblock_prologue` | 150 | 77620 |
| `regalloc` | 411 | 166800 |
| `tagged_pointer_branch` | 96 | 50312 |
| `large_semantic` | 18 | 26448 |
| `shape_collide_regalloc` | 27 | 3640 |
| `tooling_or_placement` | 0 | 0 |
| `uncategorized_failed` | 542 | 117060 |

## Generated Target Files

| File | Rows | Bytes | Purpose |
|---|---:|---:|---|
| `high_yield` | 160 | 59032 | Best untried non-Read families for future matching runs. |
| `failed_retry` | 80 | 21232 | Small failed near-misses after compiler-blocked buckets are filtered out. |
| `read_research` | 90 | 30300 | Remaining cReadBlock prologue/scheduler research targets. |
| `tagged_research` | 40 | 15672 | Tagged/nullable-pointer branch-shape research targets. |

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
| `0x0007b3dc` | 200 | 84 | `eTextureMap::Apply(int, const eCamera *, const eWorld *) const` | Apply family; manageable size |
| `0x00046ce8` | 340 | 84 | `eModelNodeController::Apply(eAnimationKey *)` | Apply family; manageable size |
| `0x000b6420` | 712 | 83 | `eParticleSystemTemplate::Write(cFile &) const` | Write family; larger target |
| `0x0003d2e8` | 748 | 83 | `eRoom::Write(cFile &) const` | Write family; larger target |
| `0x0010b764` | 764 | 83 | `gcStateInfo::Write(cOutStream &) const` | Write family; larger target |
| `0x001a5bc4` | 768 | 83 | `nwMsgVoiceUpdateStatus::Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const` | Write family; larger target |
| `0x000fca74` | 780 | 83 | `gcCamera::gcEntityFollowState::Write(cOutStream &) const` | Write family; larger target |
| `0x0001da88` | 836 | 83 | `eAudio::Write(cOutStream &) static` | Write family; larger target |
| `0x00125ff0` | 872 | 83 | `gcEntityTemplate::Write(cFile &) const` | Write family; larger target |
| `0x0004eac0` | 876 | 83 | `eMeshShape::Write(cFile &) const` | Write family; larger target |
| `0x0013b798` | 296 | 82 | `gcUIGeom::Draw(const eDrawInfo &) const` | Draw family; manageable size |
| `0x000e4e34` | 352 | 82 | `gcUIDialog::Draw(unsigned int)` | Draw family; manageable size |
| ... | ... | ... | 140 more | |

## Preview: failed_retry

| Address | Size | Score | Function | Reason |
|---|---:|---:|---|---|
| `0x002a7edc` | 292 | 206 | `gcDoEntityBipedSetShape::New(cMemPool *, cBase *) static` | near-miss 4B; New retry family; has failure snapshot; manageable size |
| `0x00213598` | 488 | 205 | `eTextureMap::AssignCopy(const cBase *)` | near-miss 5B; AssignCopy retry family; has failure snapshot; manageable size |
| `0x002a54fc` | 132 | 204 | `gcPartialBodyControllerTemplate::AssignCopy(const cBase *)` | near-miss 6B; AssignCopy retry family; has failure snapshot; manageable size |
| `0x000d8654` | 180 | 202 | `gcUIWidget::Reset(cMemPool *, bool)` | near-miss 8B; Reset retry family; has failure snapshot; manageable size |
| `0x00008cf4` | 152 | 200 | `cName::Write(cOutStream &) const` | near-miss 10B; Write retry family; has failure snapshot; manageable size |
| `0x0034adb8` | 224 | 191 | `gcValLobbyOptions::Set(float)` | near-miss 19B; Set retry family; has failure snapshot; manageable size |
| `0x002fc87c` | 244 | 189 | `gcDoSetEventEnumParam::GetText(char *) const` | near-miss 16B; has failure snapshot; tagged-pointer branch research candidate; manageable size |
| `0x0032f100` | 288 | 189 | `gcValEntityHasAnimation::GetText(char *) const` | near-miss 16B; has failure snapshot; tagged-pointer branch research candidate; manageable size |
| `0x00330eac` | 288 | 189 | `gcValEntityHasTimeSince::GetText(char *) const` | near-miss 16B; has failure snapshot; tagged-pointer branch research candidate; manageable size |
| `0x0033f480` | 288 | 189 | `gcValEntityTimeSince::GetText(char *) const` | near-miss 16B; has failure snapshot; tagged-pointer branch research candidate; manageable size |
| `0x003407ec` | 288 | 189 | `gcValEntityVariable::GetText(char *) const` | near-miss 16B; has failure snapshot; tagged-pointer branch research candidate; manageable size |
| `0x0027c258` | 108 | 188 | `gcFormatString::AssignCopy(const cBase *)` | near-miss 2B; AssignCopy retry family; manageable size |
| `0x0027169c` | 396 | 188 | `gcTableColumnShort::SetSize(int)` | near-miss 2B; has failure snapshot; manageable size |
| `0x00020168` | 496 | 188 | `eWorld::UpdateVolumeLocation(eVolume *)` | near-miss 17B; has failure snapshot; tagged-pointer branch research candidate; manageable size |
| `0x0000c0d8` | 180 | 187 | `cFactory::OnNameChanged(void)` | near-miss 3B; has failure snapshot; manageable size |
| `0x00244064` | 292 | 187 | `gcStreamedCinematic::AssignCopy(const cBase *)` | near-miss 3B; AssignCopy retry family; manageable size |
| `0x001101a8` | 112 | 186 | `gcEntityControllerTemplate::FindAttackSet(cHandleT<gcEnumeration>) const` | near-miss 4B; has failure snapshot; manageable size |
| `0x001f466c` | 568 | 186 | `eMeshShape::AssignCopy(const cBase *)` | near-miss 14B; AssignCopy retry family; has failure snapshot |
| `0x0008e714` | 44 | 184 | `eVideoPlatform::SetShadowInfo(float, float, const mPlane &) static` | near-miss 6B; has failure snapshot; manageable size |
| `0x003431e0` | 128 | 182 | `gcValExternalVariable::Evaluate(void) const` | near-miss 8B; has failure snapshot; manageable size |
| ... | ... | ... | 60 more | |

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
| `0x00151fe0` | 336 | 200 | `gcDoWhile::Read(cFile &, cMemPool *)` | cReadBlock prologue/scheduler research; body reported close |
| ... | ... | ... | 70 more | |

## Preview: tagged_research

| Address | Size | Score | Function | Reason |
|---|---:|---:|---|---|
| `0x00136f68` | 508 | 247 | `gcStaticInstance::Read(cFile &, cMemPool *)` | tagged/nullable-pointer branch-shape research candidate |
| `0x0030df90` | 452 | 200 | `gcDoUISendMessage::Read(cFile &, cMemPool *)` | tagged/nullable-pointer branch-shape research candidate |
| `0x0014f48c` | 464 | 200 | `gcDoSetValue::Read(cFile &, cMemPool *)` | tagged/nullable-pointer branch-shape research candidate |
| `0x0015c9bc` | 344 | 200 | `gcValUnaryOp::Read(cFile &, cMemPool *)` | tagged/nullable-pointer branch-shape research candidate |
| `0x0034b384` | 532 | 200 | `gcValLobbyScoreboardInfo::Read(cFile &, cMemPool *)` | tagged/nullable-pointer branch-shape research candidate |
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
| `0x00158c84` | 440 | 163 | `gcValConsoleInfo::Read(cFile &, cMemPool *)` | tagged/nullable-pointer branch-shape research candidate |
| ... | ... | ... | 20 more | |

## Readiness Notes

- Do not rerun stale Read follow-up target files. The 188B family is drained.
- Use the high-yield list for ordinary matching only after current research/doc updates land.
- Use the read and tagged lists as research queues, not broad overnight queues.
- Treat `read_cblock_prologue`, `regalloc`, and `shape_collide_regalloc` as quarantine signals unless a targeted compiler/source-shape project is active.
