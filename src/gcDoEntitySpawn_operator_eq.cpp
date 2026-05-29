// gcDoEntitySpawn::operator=(const gcDoEntitySpawn &)  @ 0x00148aa0, 5640B target, gcAll_psp.obj
//
// STATUS: structurally complete; NOT YET byte-exact. Compiles to 5648B (8B over target)
// and is byte-identical to the original through offset 0xF8. The residual: the inlined
// gcDesiredValue release path lands the tagged handle in $t1 where the original used $t0,
// plus an `ori` tail-duplication vs the original's handle-preserve `and;...;move` tail,
// repeated across the ~20 inlined dances.
//
// ROOT CAUSE (corrected 2026-05-28): this is NOT the ADR-012 TU-context register-allocation
// wall, and a compiler patch is NOT the path. The CG_LRA patch workflow EMPIRICALLY showed
// this function's codegen is context-INDEPENDENT: freezing/sweeping the monotonic routine-ID
// counter (pspcor.exe 0x5032d8) is a no-op, and prepending 2 / 40 / register-heavy / float
// functions ahead of it in the same TU produces byte-identical output (verified). The $t1 /
// tail-dup choice is therefore INTRINSIC to this reconstructed source structure, i.e.
// SOURCE-CONTROLLABLE — the correct $t0-producing structure has simply not been found yet.
// No surgical, zero-regression CG_LRA patch exists either (the decision is global to every
// function). See docs/sessions/2026-05-28.md for the full CG_LRA map and the disproof of the
// 0x5032d8-seed / ADR-012-context hypotheses for this function. Marked failed (REG_ALLOC).
//
// SIZE LEVER (2026-05-28): declaring `unsigned int handle = t0;` first in the release guard
// (base from `t0`, but the post-store null-test and vtable deref reading `handle`) forces the
// original's handle-preserve move pair and is what brings the body to ~target length; without
// it the function is 88B short. The remaining $t1-vs-$t0 / tail-dup gap is the open source problem.
//
// ODR-WARNING: split-TU file. The class is locally redeclared with only the data layout
// this TU needs; it intentionally does NOT include or modify include/eMemCard-style
// canonical headers, to avoid perturbing the allocator for already-matched siblings in
// src/gcDoEntitySpawn.cpp (per docs/research/snc-register-allocation.md).

struct gcDVSlot {
    short offset;
    short pad;
    void *fn;
};

struct gcDVRec {
    unsigned int unk0;
    char *vtbl;
};

class cMemPool {
public:
    static void *GetPoolFromPtr(const void *, void *, int);
};

class gcDesiredValue {
public:
    unsigned int m;

    gcDesiredValue &operator=(const gcDesiredValue &rhs) {
        if (&rhs.m != &this->m) {
            // RELEASE old destination value
            unsigned int t0 = m;
            int imm = t0 & 1;
            int rel_guard = 1;
            if (imm != 0) {
                rel_guard = 0;
            }
            if (rel_guard != 0) {
                unsigned int handle = t0;
                int isimm = 0;
                if (imm != 0) {
                    isimm = 1;
                }
                unsigned int base;
                if (isimm != 0) {
                    base = t0 & ~1u;
                } else {
                    base = ((gcDVRec *)t0)->unk0;
                }
                m = base | 1u;
                if (handle != 0) {
                    char *rec = ((gcDVRec *)handle)->vtbl;
                    short off = ((gcDVSlot *)(rec + 0x50))->offset;
                    void *fn = ((gcDVSlot *)(rec + 0x50))->fn;
                    ((void (*)(void *, int, void *, short))fn)(
                        (char *)handle + off, 3, fn, off);
                }
            }
            // ACQUIRE source value
            unsigned int src = rhs.m;
            int acq_guard = 1;
            int imm2 = src & 1;
            if (imm2 != 0) {
                acq_guard = 0;
            }
            if (acq_guard != 0) {
                gcDVSlot *aslot = (gcDVSlot *)(((gcDVRec *)src)->vtbl + 0x10);
                short aOff = aslot->offset;
                char *srcPlus = (char *)src + aOff;
                void *pool = cMemPool::GetPoolFromPtr(&m, (void *)(unsigned int)aOff, imm2);
                unsigned int cur = m;
                int isimm2 = 0;
                if ((cur & 1) != 0) {
                    isimm2 = 1;
                }
                unsigned int base2;
                if (isimm2 != 0) {
                    base2 = cur & ~1u;
                } else {
                    base2 = ((gcDVRec *)cur)->unk0;
                }
                void *afn = aslot->fn;
                m = ((unsigned int (*)(void *, void *, unsigned int, void *))afn)(
                    srcPlus, pool, base2, afn);
            }
        }
        return *this;
    }
};

struct cNamePOD {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
};

class gcDesiredEntity {
public:
    gcDesiredEntity &operator=(const gcDesiredEntity &);
};

class gcDesiredEntityTemplate {
public:
    gcDesiredEntityTemplate &operator=(const gcDesiredEntityTemplate &);
};

class gcDoEntitySpawn {
public:
    char pad00[0x08];                  // 0x00
    unsigned int unk08;                // 0x08
    gcDesiredEntityTemplate tmpl0C;    // 0x0C
    char pad10[0x24 - 0x10];           // pad to 0x24
    int unk24;                         // 0x24
    char pad28[0x30 - 0x28];           // pad to 0x30
    gcDesiredValue dv30;               // 0x30
    int unk34;                         // 0x34
    int unk38;                         // 0x38
    gcDesiredEntity ent3C;             // 0x3C
    char pad40[0x68 - 0x40];           // pad to 0x68
    int unk68;                         // 0x68
    cNamePOD name6C;                   // 0x6C
    gcDesiredValue dv84;               // 0x84
    gcDesiredEntity ent88;             // 0x88
    char pad8C[0xB4 - 0x8C];           // pad to 0xB4
    cNamePOD nameB4;                   // 0xB4
    gcDesiredValue dvCC;               // 0xCC
    gcDesiredValue dvD0;               // 0xD0
    gcDesiredValue dvD4;               // 0xD4
    gcDesiredValue dvD8;               // 0xD8
    unsigned char unkDC;               // 0xDC
    unsigned char unkDD;               // 0xDD
    unsigned char unkDE;               // 0xDE
    unsigned char unkDF;               // 0xDF
    unsigned char unkE0;               // 0xE0
    unsigned char unkE1;               // 0xE1
    char padE2[0xE4 - 0xE2];           // pad to 0xE4
    int unkE4;                         // 0xE4
    gcDesiredValue dvE8;               // 0xE8
    gcDesiredValue dvEC;               // 0xEC
    int unkF0;                         // 0xF0
    gcDesiredValue dvF4;               // 0xF4
    gcDesiredValue dvF8;               // 0xF8
    gcDesiredValue dvFC;               // 0xFC
    gcDesiredValue dv100;              // 0x100
    int unk104;                        // 0x104
    char pad108[0x110 - 0x108];        // pad to 0x110
    gcDesiredValue dv110;              // 0x110
    int unk114;                        // 0x114
    int unk118;                        // 0x118
    gcDesiredValue dv11C;              // 0x11C
    gcDesiredValue dv120;              // 0x120
    gcDesiredValue dv124;              // 0x124
    gcDesiredValue dv128;              // 0x128
    gcDesiredValue dv12C;              // 0x12C
    gcDesiredValue dv130;              // 0x130
    gcDesiredValue dv134;              // 0x134
    gcDesiredValue dv138;              // 0x138

    gcDoEntitySpawn &operator=(const gcDoEntitySpawn &);
};

gcDoEntitySpawn &gcDoEntitySpawn::operator=(const gcDoEntitySpawn &rhs) {
    unk08 = (unk08 & ~3u);
    unsigned int flag = rhs.unk08 & 3u;
    unk08 = unk08 | flag;
    tmpl0C = rhs.tmpl0C;

    unk24 = rhs.unk24;

    dv30 = rhs.dv30;

    unk34 = rhs.unk34;
    unk38 = rhs.unk38;
    ent3C = rhs.ent3C;

    unk68 = rhs.unk68;
    name6C = rhs.name6C;

    dv84 = rhs.dv84;
    ent88 = rhs.ent88;

    nameB4 = rhs.nameB4;

    dvCC = rhs.dvCC;
    dvD0 = rhs.dvD0;
    dvD4 = rhs.dvD4;
    dvD8 = rhs.dvD8;

    unkDC = rhs.unkDC;
    unkDD = rhs.unkDD;
    unkDE = rhs.unkDE;
    unkDF = rhs.unkDF;
    unkE0 = rhs.unkE0;
    unkE1 = rhs.unkE1;

    unkE4 = rhs.unkE4;

    dvE8 = rhs.dvE8;
    dvEC = rhs.dvEC;

    unkF0 = rhs.unkF0;

    dvF4 = rhs.dvF4;
    dvF8 = rhs.dvF8;
    dvFC = rhs.dvFC;
    dv100 = rhs.dv100;

    unk104 = rhs.unk104;

    dv110 = rhs.dv110;

    unk114 = rhs.unk114;
    unk118 = rhs.unk118;

    dv11C = rhs.dv11C;
    dv120 = rhs.dv120;
    dv124 = rhs.dv124;
    dv128 = rhs.dv128;
    dv12C = rhs.dv12C;
    dv130 = rhs.dv130;
    dv134 = rhs.dv134;
    dv138 = rhs.dv138;

    return *this;
}
