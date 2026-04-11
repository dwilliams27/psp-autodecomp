#pragma once

typedef unsigned int SceULong128 __attribute__((mode(TI)));

// mOCS: Object Coordinate System — position + orientation (64 bytes, 0x40)
// Confirmed from mOCS::AffineInverse (0x001a33b4): quadword accesses at
//   offsets 0x00, 0x10, 0x20 (basis rows) and 0x30 (position)
// Confirmed from eGeom::ApplyLocalToWorld (0x001e1e64): quadword copies at
//   offsets 0x10, 0x20, 0x30, 0x40 on destination (0x10 bias = vtable skip)
//   and offsets 0x00, 0x10, 0x20, 0x30 on source mOCS
// Confirmed from eDynamicGeom::GetSubObjectToWorld (0x001edb78): copies
//   quadwords at 0x10, 0x20, 0x30, 0x40 from source to 0x00, 0x10, 0x20, 0x30 on mOCS*
struct mOCS {
    SceULong128 row0;      // offset 0x00 — basis row 0 (right)
    SceULong128 row1;      // offset 0x10 — basis row 1 (up)
    SceULong128 row2;      // offset 0x20 — basis row 2 (forward)
    SceULong128 position;  // offset 0x30 — translation

    void AffineInverse(const mOCS &);
};
