# 007: Original Shape Context Reconstruction

**Date:** 2026-05-07
**Status:** Research complete; no production changes made
**Branch:** `research/original-shape-context`

## Goal

Follow up on the TU-context register-allocation research by testing a larger
source-side reconstruction for the shape `Collide` family before moving to
compiler-internals work.

The question was narrow: can a more original-looking class declaration and
method order steer SNC into the EBOOT register allocation for the repeated
140-byte negate-normal `Collide` wrappers?

## Method

Research artifacts were generated under
`logs/research/original_shape_context/`. That tree is intentionally ignored by
git, so this doc is the durable summary.

The pass used `tools/research/tu_context_harness.py` to compile generated
translation units, extract the exact mangled symbol, mask relocations, and
score against EBOOT bytes without editing production `src/` or
`config/functions.json`.

Two generated source families were tested:

- `eMultiSphereShape` full-order shard:
  - shadowed `eMultiSphereShape.h` as `emulti_original_order.h` because the
    harness adds the seed include directory after `-Iinclude`;
  - reordered declarations into the EBOOT-like method order;
  - added the missing `eSphereShape`, `eMultiSphereShape`, and `eCylinderShape`
    `Collide` wrappers into the same generated TU;
  - added a shadow `eCollision_original_order.h` declaration for
    `MultiSphereMultiSphere`.
- `eConvexHullShape` original-order shard:
  - moved the `eConvexHullShape` class declaration ahead of the keyframed helper
    classes so SNC emits the eConvex symbols first;
  - tested a class-first shard with the multi/capsule/convex wrappers;
  - then added the generic/box/sphere prefix wrappers to approximate the
    low-address eConvex `Collide` cluster.

The object-order hypothesis was confirmed: changing declaration order changed
the emitted `.o` symbol order. For example, the eConvex generated shard emitted
the constructor and `Collide` cluster before the keyframed `GetType` helpers,
where the current local source emits the keyframed helpers first.

## Results

### eMultiSphereShape

| Target | Best Result | Notes |
| --- | ---: | --- |
| `0x00069534` generic `Collide(eShape*)` | `113/172`, compiled symbol `176B` | Still size-divergent. |
| `0x000695e0` `Collide(eBoxShape*)` | `83/140`, compiled symbol `144B` | Still size-divergent. |
| `0x0006966c` `Collide(eSphereShape*)` | `21/140` | Same allocator drift as the other 140B wrappers. |
| `0x000696f8` `Collide(eMultiSphereShape*)` | `17/140` | Same as known best; `sched=2` did not improve. |
| `0x000697a8` `Collide(eCylinderShape*)` | `21/140` | Same as known best; `sched=2` did not improve. |

Guard rows stayed exact in the generated original-order TU:
`0x00069784`, `0x00069834`, `0x00069858`, `0x00069880`, and `0x000698a8`.

The `0x000696f8` contact-pointer barrier remains useful. Removing it worsened
that wrapper from `17/140` to `21/140`.

### eConvexHullShape

| Target | Best Result | Notes |
| --- | ---: | --- |
| `0x00071260` generic `Collide(eShape*)` | `111/172`, compiled symbol `176B` | Still size-divergent. |
| `0x0007130c` `Collide(eBoxShape*)` | `83/140`, compiled symbol `144B` | Still size-divergent. |
| `0x00071398` `Collide(eSphereShape*)` | `83/140`, compiled symbol `144B` | Same as box wrapper. |
| `0x00071424` `Collide(eMultiSphereShape*)` | `21/140` | No movement under class-first, full-prefix, or `sched=2`. |
| `0x000714b0` `Collide(eCapsuleShape*)` | `21/140` | No movement under class-first, full-prefix, or `sched=2`. |
| `0x0007153c` `Collide(eConvexHullShape*)` | `21/140` | No movement under class-first, full-prefix, or `sched=2`. |

Guard rows for `0x00071640` `GetInertialTensor` and `0x00071688`
`GetVolume` stayed exact in both eConvex generated shards.

## Conclusion

This pass remains **Tier C**. Larger original-looking source context changed
emitted symbol order and preserved nearby matched guards, but did not improve
the failed wrappers. The repeated signature is still the same SNC allocator and
scheduler drift: call-setup moves differ, and the negate-normal loop uses the
opposite `a0`/`a1` counter-pointer allocation from EBOOT.

This is useful negative evidence. It argues against spending overnight matching
time on more local reorderings, pragma toggles, or standard permuter passes for
these shape `Collide` wrappers. The remaining source-side route would be a much
larger full class/TU reconstruction with heavy methods such as `Write`, `Read`,
destructors, casts, and AABB bodies restored before the `Collide` cluster; that
is significantly more work and still has low confidence after this pass.

Recommended next choices:

- Treat these 140-byte shape `Collide` wrappers as compiler-blocked for normal
  matching runs unless new evidence appears.
- If we want to keep pushing this family, move to the pspcor allocator/scheduler
  research tracked in `docs/enhancements.md`.
- For bytes-per-minute overnight runs, target other families and exclude this
  shape `Collide` cluster from repeated permuter/reordering attempts.
