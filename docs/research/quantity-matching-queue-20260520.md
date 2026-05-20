# Quantity Matching Queue: 2026-05-20

Purpose: maximize overnight match count by feeding agents many small/medium untried game/source methods while avoiding families that recent runs show are poor broad-run bets.

## Queue

- Target file: `config/targets_quantity_untried_20260520.json`
- Rows: 760
- Total bytes: 210328
- Size mix: `129-256` 295, `257-512` 385, `65-128` 79, `<=64` 1

## Selection Rules

- Start from current `config/functions.json` rows with `match_status == untried`.
- Keep only class-qualified source methods (`Class::Method`) with object provenance.
- Exclude known non-source/import/runtime helper targets via `prep_exclusion_reason`.
- Exclude `std::` runtime/library targets.
- Cap at 512 bytes for throughput.
- Exclude broad-run sink families by prefix: `VisitReferences`, `Read*`, `Collide*`, `GetText*`, `operator=`, `SetSize*`, `Draw*`, `Apply*`, `Cull*`, and `Replicate*`.
- Rank by observed family success, exact-size matched method templates, same-class matched siblings, and smaller function size.

This queue was generated from the current DB with the scoring rules above; the committed JSON is the run input. Regenerate only after rerunning the scoring against the current DB and revalidating row counts/statuses.

## Top Categories

| Category | Rows |
|---|---:|
| `Evaluate` | 16 |
| `Update` | 12 |
| `Constructor` | 7 |
| `OnMemPoolReset` | 7 |
| `CastRay` | 7 |
| `Initialize` | 6 |
| `RemoveFromWorld` | 5 |
| `GetEmbedContacts` | 5 |
| `GetControllerType` | 5 |
| `Load` | 4 |
| `IsUpdateEmpty` | 4 |
| `Free` | 4 |
| `GetSweptContacts` | 4 |
| `GetCollisionHandler` | 4 |
| `CreateAndResetInstance` | 4 |
| `Run` | 4 |
| `Activate` | 4 |
| `AddToWorld` | 4 |
| `CastSphere` | 4 |
| `ProcessCollision` | 4 |
| `Create` | 4 |
| `RemoveAll` | 4 |
| `GetSubObject` | 4 |
| `GetRelativeFilename` | 4 |
| `Intersect` | 4 |
| `Contains` | 3 |
| `GetRelationships` | 3 |
| `Send` | 3 |
| `OnActivated` | 3 |
| `CalcEmbeddedContact` | 3 |

## Preview

| Address | Size | Score | Function | Reason |
|---|---:|---:|---|---|
| `0x000f7aa4` | 144 | 140 | `gcBackgroundLoader::Load(const cGUIDT<gcRegion> *, int *)` | family observed rate 100%; 2 matched same-class method(s); compact; quantity-run preferred family |
| `0x000f7b80` | 156 | 140 | `gcBackgroundLoader::Load(int, const gcMap::cObjectLoad *)` | family observed rate 100%; 2 matched same-class method(s); compact; quantity-run preferred family |
| `0x0000530c` | 160 | 140 | `cFile::Close(bool)` | family observed rate 100%; 6 matched family exemplar(s); compact; quantity-run preferred family |
| `0x001a6148` | 236 | 140 | `nwConnection::Close(void)` | family observed rate 100%; 6 matched family exemplar(s); compact; quantity-run preferred family |
| `0x0013ab9c` | 108 | 136 | `gcUIEditBox::IsUpdateEmpty(bool, bool) const` | family observed rate 100%; small; quantity-run preferred family |
| `0x00044ac4` | 120 | 136 | `eStaticMesh::Free(void)` | family observed rate 100%; small; quantity-run preferred family |
| `0x00021c50` | 148 | 135 | `eBspTree::Contains(const mVec3 &) const` | family observed rate 100%; 1 matched same-class method(s); compact; quantity-run preferred family |
| `0x000450d4` | 296 | 134 | `eDynamicGeom::Detach(void)` | family observed rate 100%; 5 matched family exemplar(s); medium; quantity-run preferred family |
| `0x00067f78` | 392 | 134 | `eSphereShape::GetSweptContacts(int, const mSphere *, const mCollideInfo *, const eCollisionInfo &, eContactCollector *) const` | family observed rate 100%; 6 matched family exemplar(s); medium; quantity-run preferred family |
| `0x00069000` | 436 | 134 | `eMultiSphereShape::GetSweptContacts(int, const mSphere *, const mCollideInfo *, const eCollisionInfo &, eContactCollector *) const` | family observed rate 100%; 6 matched family exemplar(s); medium; quantity-run preferred family |
| `0x0006d6ac` | 444 | 134 | `eBoxShape::GetSweptContacts(int, const mSphere *, const mCollideInfo *, const eCollisionInfo &, eContactCollector *) const` | family observed rate 100%; 6 matched family exemplar(s); medium; quantity-run preferred family |
| `0x0006a530` | 448 | 134 | `eCapsuleShape::GetSweptContacts(int, const mSphere *, const mCollideInfo *, const eCollisionInfo &, eContactCollector *) const` | family observed rate 100%; 6 matched family exemplar(s); medium; quantity-run preferred family |
| `0x001e832c` | 76 | 131 | `eInputJoystick::eInputState::eInputState(void)` | family observed rate 85%; 289 matched family exemplar(s); small; quantity-run preferred family |
| `0x0013a5d8` | 108 | 131 | `gcUIEditBox::gcUIEditBox(cBase *)` | family observed rate 85%; 289 matched family exemplar(s); small; quantity-run preferred family |
| `0x00021250` | 120 | 131 | `eSound::eSound(cBase *)` | family observed rate 85%; 289 matched family exemplar(s); small; quantity-run preferred family |
| `0x0003ecfc` | 172 | 130 | `eRoom::Free(void)` | family observed rate 100%; compact; quantity-run preferred family |
| `0x0003cbac` | 184 | 130 | `eRoomAABBTree::RemoveFromWorld(eWorld *)` | family observed rate 100%; compact; quantity-run preferred family |
| `0x00061edc` | 192 | 130 | `eWeatherSystem::RemoveFromWorld(void)` | family observed rate 100%; compact; quantity-run preferred family |
| `0x000f3a88` | 200 | 130 | `gcMap::RemoveFromWorld(void)` | family observed rate 100%; compact; quantity-run preferred family |
| `0x000dc9c4` | 212 | 130 | `gcUIWidget::IsUpdateEmpty(bool, bool) const` | family observed rate 100%; compact; quantity-run preferred family |
| `0x000e8d58` | 212 | 130 | `gcUIDialog::IsUpdateEmpty(bool) const` | family observed rate 100%; compact; quantity-run preferred family |
| `0x00068c34` | 228 | 130 | `eMultiSphereShape::GetCollisionHandler(const mCollideInfo &) const` | family observed rate 100%; compact; quantity-run preferred family |
| `0x00068d18` | 228 | 130 | `eMultiSphereShape::GetCollisionHandler(const mVec3 &) const` | family observed rate 100%; compact; quantity-run preferred family |
| `0x0006a198` | 228 | 130 | `eCapsuleShape::GetCollisionHandler(const mCollideInfo &) const` | family observed rate 100%; compact; quantity-run preferred family |
| `0x0006a27c` | 228 | 130 | `eCapsuleShape::GetCollisionHandler(const mVec3 &) const` | family observed rate 100%; compact; quantity-run preferred family |
| `0x000453b4` | 252 | 130 | `eDynamicGeom::UpdateLocalToWorld(void)` | family observed rate 100%; compact; quantity-run preferred family |
| `0x000f8a80` | 88 | 129 | `gcCamera::OnMemPoolReset(const cMemPool *, unsigned int)` | family observed rate 83%; 5 matched family exemplar(s); small; quantity-run preferred family |
| `0x000221e8` | 468 | 129 | `eBspTree::Contains(int, const eShape *, const mOCS &, bool *, bool *) const` | family observed rate 100%; 1 matched same-class method(s); medium; quantity-run preferred family |
| `0x00001f20` | 192 | 125 | `cThread::cThread(cThread::cThreadPriority, unsigned int, void *, unsigned int)` | family observed rate 85%; 289 matched family exemplar(s); compact; quantity-run preferred family |
| `0x0008d084` | 192 | 125 | `eAudioPlatform::eStreamThread::eStreamThread(void)` | family observed rate 85%; 289 matched family exemplar(s); compact; quantity-run preferred family |
| `0x001a6420` | 244 | 125 | `nwConnection::nwConnection(nwSocket *, nwConnectionHandle, int, const nwAddress &, void (*)(nwConnectionHandle, nwConnection::nwConnectionError))` | family observed rate 85%; 289 matched family exemplar(s); compact; quantity-run preferred family |
| `0x00040f38` | 280 | 124 | `eGeomTemplate::CreateAndResetInstance(cMemPool *, cBase *, const eGeomTemplate *, eGeom **) static` | family observed rate 100%; medium; quantity-run preferred family |
| `0x0010ff18` | 280 | 124 | `gcEntityControllerTemplate::CreateAndResetInstance(cMemPool *, gcEntity *, const gcEntityControllerTemplate *, gcEntityController **) static` | family observed rate 100%; medium; quantity-run preferred family |
| `0x0013e5b0` | 280 | 124 | `gcPartialEntityControllerTemplate::CreateAndResetInstance(cMemPool *, gcEntity *, const gcPartialEntityControllerTemplate *, gcPartialEntityController **) static` | family observed rate 100%; medium; quantity-run preferred family |
| `0x00105f64` | 288 | 124 | `gcGame::Run(void) static` | family observed rate 100%; medium; quantity-run preferred family |
| `0x0004e760` | 308 | 124 | `eDynamicMesh::Free(void)` | family observed rate 100%; medium; quantity-run preferred family |
| `0x00003558` | 324 | 124 | `cDynamicMemAllocator::Free(void *)` | family observed rate 100%; medium; quantity-run preferred family |
| `0x00147de4` | 324 | 124 | `gcSubGeomController::CreateAndResetInstance(cMemPool *, gcEntity *, const gcEntityGeomConfig *, eDynamicGeom *, gcSubGeomController **) static` | family observed rate 100%; medium; quantity-run preferred family |
| `0x0003df4c` | 328 | 124 | `eRoom::RemoveFromWorld(eWorld *)` | family observed rate 100%; medium; quantity-run preferred family |
| `0x0003e094` | 340 | 124 | `eRoom::Contains(const mVec3 &) const` | family observed rate 100%; medium; quantity-run preferred family |

## Recommended Launch

Commit this queue first and launch only once `git status --short --branch` shows clean `main`. Use Codex-only unless `scripts/check_agent_auth.sh` passes for Claude immediately before launch. Targeted mode keeps the queue away from known sink families; `--batch-size 3` recovers some throughput versus the targeted default of 2.

```bash
./tools/run_overnight.sh --hours 10 \
  --targets config/targets_quantity_untried_20260520.json \
  --batch-size 3 \
  --workers 4 \
  --identities codex/gpt-5.5/medium,codex/gpt-5.5/high \
  --variants tier12b
```
