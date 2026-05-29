// gcDoEntityCastRay::operator=(const gcDoEntityCastRay &)  @ 0x00142c24, 4360B, gcAll_psp.obj
//
// Split-TU draft. Same family as src/gcDoEntitySpawn_operator_eq.cpp; reuses the verified
// gcDesiredValue inline dance verbatim. New member types: gcEvent (operator= call @0x3C) and
// cArrayBase<gcDesiredEntityHelper> (operator= call @0xD4).
//
// LAYOUT MAP (s2=this/dst, s3=rhs/src):
//   0x08   unk08      flag word: (unk08 & ~3) | (rhs.unk08 & 3)
//   0x0C   pod0C      6-word POD copy (24B)        0x0C..0x23
//   0x24   pod24      6-word POD copy (24B)        0x24..0x3B
//   0x3C   ev3C       gcEvent  -> operator=
//   0x58   ent58      gcDesiredEntity -> operator=
//   0x8C   unk8C      single word = rhs
//   0x90   ent90      gcDesiredEntity -> operator=
//   0xBC   unkBC      single word = rhs
//   0xC0   dvC0       gcDesiredValue (addr cached s0)
//   0xC4   dvC4       gcDesiredValue (addr cached s4)
//   0xC8   dvC8       gcDesiredValue (addr cached s5)
//   0xCC   dvCC       gcDesiredValue (addr cached s6)
//   0xD0   dvD0       gcDesiredValue (addr cached s7)
//   0xD4   arrD4      cArrayBase<gcDesiredEntityHelper> -> operator=
//   0xD8   dvD8       gcDesiredValue (addr cached s8, beql self-check)
//   0xDC   dvDC       gcDesiredValue (addr from stack sp+0,  beql self-check)
//   0xE0   dvE0       gcDesiredValue (sp+4,  beql)
//   0xE4   dvE4       gcDesiredValue (sp+8,  beql)
//   0xE8   dvE8       gcDesiredValue (sp+12, beql)
//   0xEC   dvEC       gcDesiredValue (sp+16, beq plain)
//   0xF0   unkF0      single word = rhs
//   0xF4   b F4,F5,F6 3 bytes (unsigned char)
//   0x100  b 100,101  2 bytes (signed char)
//   0x104  unk104     single word
//   0x108  unk108     single word
//   0x10C  dv10C      gcDesiredValue (sp+20, beql)
//   0x110  dv110      gcDesiredValue (sp+24, beql)
//   0x114  dv114      gcDesiredValue (sp+28, beql)
//   0x118  dv118      gcDesiredValue (sp+32, beql)
//   0x11C  dv11C      gcDesiredValue (sp+36, bnel)
//   0x120  b 120      1 byte (unsigned char), final

typedef void (*relFnT)(void *, void *);
typedef int (*aFnT)(void *, void *, unsigned int);

struct gcDVSlot { short offset; short pad; relFnT fn; };
struct gcDVASlot { short offset; short pad; aFnT fn; };
struct gcDVRec { unsigned int unk0; char *vtbl; };

class cMemPool {
public:
    static void *GetPoolFromPtr(const void *);
};

class gcDesiredValue {
public:
    unsigned int m;

    gcDesiredValue &operator=(const gcDesiredValue &rhs) {
        if (&rhs.m != &this->m) {
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
                unsigned int newval;
                if (isimm != 0) {
                    int base = t0 & ~1u;
                    newval = base | 1u;
                } else {
                    unsigned int base = ((gcDVRec *)t0)->unk0;
                    newval = base | 1u;
                }
                m = newval;
                if (handle != 0) {
                    char *rec = ((gcDVRec *)handle)->vtbl;
                    gcDVSlot *slot = (gcDVSlot *)(rec + 0x50);
                    slot->fn((char *)handle + slot->offset, (void *)3);
                }
            }
            unsigned int src = rhs.m;
            int acq_guard = 1;
            int imm2 = src & 1;
            if (imm2 != 0) {
                acq_guard = 0;
            }
            if (acq_guard != 0) {
                gcDVASlot *aslot = (gcDVASlot *)(((gcDVRec *)src)->vtbl + 0x10);
                short aOff = aslot->offset;
                char *srcPlus = (char *)src + aOff;
                void *pool = cMemPool::GetPoolFromPtr(&m);
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
                    __asm__ volatile("" ::: "memory");
                }
                m = aslot->fn(srcPlus, pool, base2);
            }
        }
        return *this;
    }
};

struct cNamePOD { int f0, f4, f8, fC, f10, f14; };

class gcEvent {
public:
    gcEvent &operator=(const gcEvent &);
};

class gcDesiredEntity {
public:
    gcDesiredEntity &operator=(const gcDesiredEntity &);
};

class gcDesiredEntityHelper {};

template <class T>
class cArrayBase {
public:
    cArrayBase &operator=(const cArrayBase &);
};

class gcDoEntityCastRay {
public:
    char pad00[0x08];                            // 0x00
    unsigned int unk08;                          // 0x08
    cNamePOD pod0C;                              // 0x0C
    cNamePOD pod24;                              // 0x24
    gcEvent ev3C;                                // 0x3C
    char pad3C[0x58 - (0x3C + sizeof(gcEvent))]; // pad to 0x58
    gcDesiredEntity ent58;                       // 0x58
    char pad58[0x8C - (0x58 + sizeof(gcDesiredEntity))];
    int unk8C;                                   // 0x8C
    gcDesiredEntity ent90;                       // 0x90
    char pad90[0xBC - (0x90 + sizeof(gcDesiredEntity))];
    int unkBC;                                   // 0xBC
    gcDesiredValue dvC0;                         // 0xC0
    gcDesiredValue dvC4;                         // 0xC4
    gcDesiredValue dvC8;                         // 0xC8
    gcDesiredValue dvCC;                         // 0xCC
    gcDesiredValue dvD0;                         // 0xD0
    cArrayBase<gcDesiredEntityHelper> arrD4;     // 0xD4
    char padD4[0xD8 - (0xD4 + sizeof(cArrayBase<gcDesiredEntityHelper>))];
    gcDesiredValue dvD8;                         // 0xD8
    gcDesiredValue dvDC;                         // 0xDC
    gcDesiredValue dvE0;                         // 0xE0
    gcDesiredValue dvE4;                         // 0xE4
    gcDesiredValue dvE8;                         // 0xE8
    gcDesiredValue dvEC;                         // 0xEC
    int unkF0;                                   // 0xF0
    unsigned char unkF4;                         // 0xF4
    unsigned char unkF5;                         // 0xF5
    unsigned char unkF6;                         // 0xF6
    char padF7[0x100 - 0xF7];                    // pad to 0x100
    signed char unk100;                          // 0x100
    signed char unk101;                          // 0x101
    char pad102[0x104 - 0x102];                  // pad to 0x104
    int unk104;                                  // 0x104
    int unk108;                                  // 0x108
    gcDesiredValue dv10C;                        // 0x10C
    gcDesiredValue dv110;                        // 0x110
    gcDesiredValue dv114;                        // 0x114
    gcDesiredValue dv118;                        // 0x118
    gcDesiredValue dv11C;                        // 0x11C
    unsigned char unk120;                        // 0x120

    gcDoEntityCastRay &operator=(const gcDoEntityCastRay &);
};

gcDoEntityCastRay &gcDoEntityCastRay::operator=(const gcDoEntityCastRay &rhs) {
    unk08 = (unk08 & ~3u);
    unsigned int flag = rhs.unk08 & 3u;
    unk08 = unk08 | flag;

    pod0C = rhs.pod0C;
    pod24 = rhs.pod24;

    ev3C = rhs.ev3C;
    ent58 = rhs.ent58;

    unk8C = rhs.unk8C;
    ent90 = rhs.ent90;

    unkBC = rhs.unkBC;

    dvC0 = rhs.dvC0;
    __asm__ volatile("" ::: "memory");
    dvC4 = rhs.dvC4;
    dvC8 = rhs.dvC8;
    dvCC = rhs.dvCC;
    dvD0 = rhs.dvD0;

    arrD4 = rhs.arrD4;

    dvD8 = rhs.dvD8;
    dvDC = rhs.dvDC;
    dvE0 = rhs.dvE0;
    dvE4 = rhs.dvE4;
    dvE8 = rhs.dvE8;
    dvEC = rhs.dvEC;

    unkF0 = rhs.unkF0;

    unkF4 = rhs.unkF4;
    unkF5 = rhs.unkF5;
    unkF6 = rhs.unkF6;

    unk100 = rhs.unk100;
    unk101 = rhs.unk101;

    unk108 = rhs.unk108;
    unk104 = rhs.unk104;

    dv10C = rhs.dv10C;
    __asm__ volatile("" ::: "memory");
    dv110 = rhs.dv110;
    dv114 = rhs.dv114;
    dv118 = rhs.dv118;
    dv11C = rhs.dv11C;
    __asm__ volatile("" ::: "memory");

    unk120 = rhs.unk120;
    __asm__ volatile("" ::: "memory");

    return *this;
}
