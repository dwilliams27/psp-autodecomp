// ODR-WARNING: this TU intentionally redeclares eCompoundShape with only the
// members needed for Read(cFile &, cMemPool *). Including eCompoundShape.h
// would perturb matched sibling TUs in src/eCompoundShape.cpp.

typedef unsigned int SceULong128 __attribute__((mode(TI)));

struct mOCS {
    SceULong128 row0;
    SceULong128 row1;
    SceULong128 row2;
    SceULong128 position;
};

class cFile;
class cMemPool;
class eShape;

class cReadBlock {
public:
    int _data[5];
};

extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);
void cFile_SetCurrentPos(void *, unsigned int);
int eShape_Read(void *, cFile &, cMemPool *);
void eShape_ComputeMinAxis(void *);

class cBaseArray {
public:
    void Read(cReadBlock &);
};

extern mOCS D_00038578;
extern int D_000385B8;

struct eCompoundShape_BoxEntry {
    short offset;
    short pad;
    void (*fn)(void *, void *, const mOCS *);
};

struct eCompoundShape_ReadLocals {
    mOCS input;
    int rb[5];
    char pad54[0x0C];
    SceULong128 box[2];
    char pad80[0x6C];
};

class eCompoundShape {
public:
    char _pad[0x80];
    eShape **mSubShapes;
    eCompoundShape *mSelf;

    int Read(cFile &, cMemPool *);
};

#pragma control sched=1

// eCompoundShape::Read(cFile &, cMemPool *) - 0x00072b38
int eCompoundShape::Read(cFile &file, cMemPool *pool) {
    eCompoundShape_ReadLocals locals;
    register int *rbp __asm__("$19") = locals.rb;
    register int result __asm__("$20");
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));

    __0oKcReadBlockctR6FcFileUib(rbp, file, 1, true);
    if (rbp[3] != 1 || eShape_Read(this, file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rbp[0], rbp[1]);
        __0oKcReadBlockdtv(rbp, 2);
        return 0;
    }

    ((cBaseArray *)((char *)this + 0x80))->Read(*(cReadBlock *)rbp);
    *(float *)((char *)this + 0x74) = 0.0f;

    int index = 0;
    int offset = 0;
    eShape **subShapes = *(eShape ***)((char *)this + 0x80);
    while (true) {
        int count = 0;
        if (subShapes != 0) {
            count = ((int *)subShapes)[-1];
        }
        if (index >= count) {
            break;
        }

        eShape *shape = *(eShape **)((char *)subShapes + offset);
        int lenBits;
        __asm__ volatile(
            "lv.q C120, 0x40(%1)\n"
            "vdot.t S100, C120, C120\n"
            "vsqrt.s S100, S100\n"
            "mfv %0, S100\n"
            : "=r"(lenBits)
            : "r"(shape)
            : "memory"
        );
        float len = *(float *)&lenBits;
        float candidate = len + *(float *)((char *)shape + 0x74);
        float current = *(float *)((char *)this + 0x74);
        if (!(current <= candidate)) {
            candidate = current;
        }
        *(float *)((char *)this + 0x74) = candidate;
        *(unsigned char *)((char *)shape + 0x50) = (unsigned char)result;

        index++;
        offset += 4;
        subShapes = *(eShape ***)((char *)this + 0x80);
    }

    if (D_000385B8 == 0) {
        D_000385B8 = 1;
        void *identity = &D_00038578;
        __asm__ volatile(
            "mfc1 $a3, $f12\n"
            "mfc1 $t0, $f12\n"
            "mfc1 $t3, $f12\n"
            "mtv $a3, S120\n"
            "mtv $t0, S121\n"
            "mtv $t3, S122\n"
            "sv.q C120, 0x80($sp)\n"
            "lui $a3, 0x3f80\n"
            "mtc1 $a3, $f13\n"
            "mfc1 $a3, $f13\n"
            "mfc1 $t0, $f12\n"
            "mfc1 $t3, $f12\n"
            "mtv $a3, S130\n"
            "mtv $t0, S131\n"
            "mtv $t3, S132\n"
            "sv.q C130, 0xc0($sp)\n"
            "mfc1 $a3, $f12\n"
            "mfc1 $t0, $f13\n"
            "mfc1 $t3, $f12\n"
            "mtv $a3, S200\n"
            "mtv $t0, S201\n"
            "mtv $t3, S202\n"
            "sv.q C200, 0xd0($sp)\n"
            "mfc1 $a3, $f12\n"
            "mfc1 $t0, $f12\n"
            "mfc1 $t3, $f13\n"
            "mtv $a3, S210\n"
            "mtv $t0, S211\n"
            "mtv $t3, S212\n"
            "sv.q C210, 0xe0($sp)\n"
            "sv.q C130, 0x90($sp)\n"
            "sv.q C200, 0xa0($sp)\n"
            "sv.q C210, 0xb0($sp)\n"
            "sv.q C130, 0x0(%0)\n"
            "sv.q C200, 0x10(%0)\n"
            "sv.q C210, 0x20(%0)\n"
            "sv.q C120, 0x30(%0)\n"
            :
            : "r"(identity)
            : "$a3", "$t0", "$t3", "$f13", "memory"
        );
    }

    __asm__ volatile("" ::: "memory");
    locals.input = D_00038578;

    char *vtable = *(char **)((char *)this + 4);
    eCompoundShape_BoxEntry *entry = (eCompoundShape_BoxEntry *)(vtable + 0xC8);
    entry->fn((char *)this + entry->offset, locals.box, &locals.input);

    __asm__ volatile(
        "lv.q C120, 0x0(%1)\n"
        "lv.q C130, 0x10(%1)\n"
        "vadd.t C200, C120, C130\n"
        "sv.q C200, 0x90(%0)\n"
        "lui $a0, 0x3f00\n"
        "mtc1 $a0, $f12\n"
        "mfc1 $a0, $f12\n"
        "mtv $a0, S100\n"
        "vscl.t C200, C200, S100\n"
        "sv.q C200, 0x90(%0)\n"
        "vsub.t C120, C130, C120\n"
        "sv.q C120, 0xa0(%0)\n"
        "vscl.t C120, C120, S100\n"
        "sv.q C120, 0xa0(%0)\n"
        :
        : "r"(this), "r"(locals.box)
        : "$a0", "$f12", "memory"
    );
    eShape_ComputeMinAxis(this);
    __0oKcReadBlockdtv(rbp, 2);
    return result;
}

#pragma control sched=2
