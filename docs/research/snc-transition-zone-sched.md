# SNC Transition-Zone Scheduling

Status: 2026-04-30. This note supersedes the older coarse rule that
`0x040000-0x06e000` is all `sched=2` and `0x06e000+` is all
`sched=1`.

## Summary

`eAll_psp.obj` is mixed at class and sometimes method granularity.
Address is a useful prior, not a rule. The current best operating
model is:

- Default to `sched=2` for early eAll engine code.
- Prefer local `#pragma control sched=N` around individual methods
  when neighboring matched methods prove mixed scheduling.
- Use broad Makefile `ECFLAGS` overrides only after a class has
  multiple non-trivial `sched=1` matches and no known `sched=2`
  exceptions.
- For transition-zone retries, treat `sched` as an A/B axis alongside
  source shape. Many failures are register/VFPU/frame issues even when
  the sched mode is known.

## Address Bands

| Band | Classes | Likely sched | Confidence | Notes |
|---|---|---:|---:|---|
| `0x040000-0x046e3c` | `eRoomSet`, `ePortal`, `eStaticGeom`, `eStaticModel`, `eStaticMesh`, `eDynamicGeom` | mostly `2` | High | Many matched default functions. Recent `eStaticModel::AddDrawInfos` failures look source-shape/register related; `sched=1` is worth A/B but not proven. |
| `0x046e3c-0x04b0f8` | `eDynamicModel` | mixed | High | Current source uses local `sched=2` and `sched=1` pragmas; a broad Makefile override exists, so matched source must reset exceptions locally. |
| `0x04b0f8-0x050524` | `ePhysics*Config/Template`, `eDynamicMesh*`, `eMeshShape*` | mixed, with `sched=1` pockets | High | Strongest refutation of address-only sched=2. `ePhysicsControllerTemplate::Write` and several dynamic mesh/node methods need `sched=1`; ctor-like code may remain `sched=2`. |
| `0x050524-0x05ff54` | `eHeightmapShape`, `eHeightmap`, `eSurface*`, `eLightGrid`, `eNavMesh`, lights | mixed | Medium | `eHeightmapShape::{Write,Read}` and many collide thunks need `sched=1`; ctor and trivial stubs can be `sched=2`. Older finetune probes here only proved specific methods, not the whole band. |
| `0x05ff54-0x06d368` | `eWeather*`, `eBiped*`, `eSphere/MultiSphere/Capsule`, `eSimulated*` | mixed, leaning `2` before `0x069000` | Medium | Some probes support `sched=2`, while `eEmbedVolumeCollisionHandler::Run` and constraint/motor neighbors show `sched=1` pockets. |
| `0x06d368-0x06e7bc` | `eBoxShape`, `eDragAreaUtil`, `eConvexHullUtil` | unknown boundary | Low/Med | Sparse evidence. VFPU failures here are not clean sched probes. |
| `0x06e7bc-0x080000` | `eCollisionConstraint`, `eConstraintSolver`, `eTriangle/Convex/Compound/Cylinder/Keyframed`, `eTextureMap`, `eProjector`, `eShadow` | mostly `1` | High | Many exact or materially closer matches under `sched=1`. |
| `0x080000-0x08f000` | bump/filter/framebuffer/materials, `eRenderSurface`, `eVRAMMgr`, `eVideoPlatform`, dynamic mesh vis | mostly `1`, with exceptions | High | Large matched sample. `eVideoPlatform::SetShadowInfo` is a negative control where sched does not explain the diff. |

## Focus Classes

| Class / methods | Evidence | Retry guidance |
|---|---|---|
| `eStaticModelTemplate`, `eStaticModel` normal methods | Matched under default `sched=2`; no local pragmas needed so far. | Use `sched=2` first. A/B `eStaticModel::AddDrawInfos` with `sched=1`, but expect source-shape work. |
| `ePhysicsControllerTemplate` | `Write` and `CreateAndResetInstance` use local `sched=1`; ctor matches `sched=2`; dtor note says manual `sched=1` was exact. | Retry `Read` and dtor with `sched=1`; keep ctor-like methods `sched=2`. |
| `eHeightmapShape` | `Write`, `Read`, and non-trivial collide thunks sit under `sched=1`; ctor is explicitly `sched=2`. | Retry non-trivial collision methods with `sched=1`; keep ctor/trivial stubs `sched=2`. |
| `eBipedController` | `Write`, `Read`, dtor, `SetPosition`, `ApplyImpulse`, and callback thunks use or needed `sched=1`; ctor, `InvalidateCacheEntries`, and `EnableWallWalk` use `sched=2`. | Do not apply a blind class-wide override. Use method-local pragmas. `PostUpdate`, `SetCollisionMask`, and ctor are not sched-only failures. |
| `eBodyWorldConstraint` | ctor matches default `sched=2`; `Write`, `Read`, dtor, `OnPositionChanged`, `AssignCopy`, and `New` use local `sched=1`. | Retry `Initialize` with `sched=1`. |
| `eSimpleMotor` | ctor matches default `sched=2`; `Write`, `Read`, dtor, `Initialize` use local `sched=1`. | Retry `Apply` with `sched=1`. |

## Best Sched-Focused Experiments

1. `eRenderSurface` `0x0008d5b4`, `0x0008d63c`, `0x0008d74c`: rerun with `sched=1`; dtor/uninit are documented 0-diff and `Initialize` was down to 4 bytes.
2. `eTextureFilter` `0x0007fa50`, `0x0007fa9c`, `0x0007fb90`: retry with `sched=1`; notes mention missing override.
3. `eDynamicMeshVisTriList::Read` `0x0008eb64`: retry with `sched=1`; `sched=2` produced a large structural diff.
4. `eCylinderShape` thunks `0x000743d0`, `0x00074400`, `0x00074430`, `0x00074460`, `0x00074858`: use `sched=1` calibration cases.
5. `eHeightmapShape` collide thunks `0x000513b8`, `0x000513e4`, `0x00051410`: retry with `sched=1`.
6. `ePhysicsControllerTemplate` `0x0004b918`, `0x0004bab4`: transition-zone `sched=1` island.
7. `eDynamicMesh::PlatformFree` `0x0004e1fc`: use `sched=1`; remaining issue is prologue/source shape.
8. `eDynamicMeshExtrudedShadowEdge` `0x0004d260`, `0x0004d2ac`: early-zone `sched=1` calibration pair.
9. `eEmbedVolumeCollisionHandler::Run` `0x00069cdc`: boundary probe below `0x06e000` with `sched=1` evidence.
10. `eConstraintSolver` `0x0006f834`, `0x0006f8b0`: retry with `sched=1`; notes say `sched=2` had wrong prologue/control shape.

## Operational Notes

- `#pragma control sched=1` / `sched=2` is preferred over adding
  broad Makefile overrides while a class has known mixed methods.
- If a function is already within roughly 5-20 bytes, preserve the
  current source and retry from the exact near-miss snapshot instead
  of starting from the prose note alone.
- When a function contains VFPU loads/stores, callback thunks, or
  long-lived locals, do not classify it as a sched failure unless both
  sched modes have been compared. Those often fail on register
  allocation or delay-slot/source-shape issues.
