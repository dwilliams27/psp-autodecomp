#pragma once

class cMemPool;

// eContactFeature: 8-byte struct used as key for collision constraint allocation.
// Allocate copies 8 bytes from eContactFeature into constraint at +0x08..+0x0F.
struct eContactFeature {
    int bodyA;      // 0x00
    int bodyB;      // 0x04
};

// eCollisionConstraint is a pool-allocated node in a free-list.
// Layout inferred from Allocate (0x6ea60) and Free (0x6eaa4):
//   +0x00: next pointer (free-list link)
//   +0x04: float (cleared to 0.0 on allocate)
//   +0x08: eContactFeature (8 bytes, copied from arg)
//   +0x10: byte flag (cleared to 0 on allocate)
// Total known size: at least 0x14 bytes (20 bytes per entry).
// Initialize builds the pool from cMemPool.
// The pool entries are 20 bytes each (stride 0x14 in Initialize loop).
struct eCollisionConstraint {
    eCollisionConstraint *mNext;    // 0x00 - free-list link
    float mValue;                   // 0x04
    eContactFeature mFeature;       // 0x08
    unsigned char mFlags;           // 0x10
    char _pad11[3];                 // 0x11

    static int Initialize(cMemPool *);
    static eCollisionConstraint *Allocate(const eContactFeature &);
    static void Free(eCollisionConstraint *);
};
