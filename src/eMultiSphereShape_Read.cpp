// ODR-WARNING: split-TU local redeclaration for adding Read without
// changing the matched eMultiSphereShape translation unit.

class cBase;
class cFile;
class cMemPool;

class cReadBlock {
public:
};

class cFileSystem {
public:
    static void Read(void *, void *, unsigned int);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);
int eShape_Read(void *, cFile &, cMemPool *);
void eShape_ComputeMinAxis(void *);

class eMultiSphereShape {
public:
    char _pad[0x80];
    float radius;
    float halfLength;

    int Read(cFile &, cMemPool *);
};

#pragma control sched=1

// eMultiSphereShape::Read(cFile &, cMemPool *) - 0x00068934
int eMultiSphereShape::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$17");
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 1, true);
    int scratch[23];
    __asm__ volatile("" : "+m"(scratch));

    if ((unsigned int)rb[3] != 1 || eShape_Read(this, file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    void *h = *(void **)rb[0];
    __asm__ volatile("" : "+r"(h));
    cFileSystem::Read(h, &radius, 4);
    h = *(void **)rb[0];
    __asm__ volatile("" : "+r"(h));
    cFileSystem::Read(h, &halfLength, 4);

    *(float *)((char *)this + 0x74) = radius + halfLength;
    eShape_ComputeMinAxis(this);

    register float r __asm__("$f13") = radius;
    __asm__ volatile("swc1 %0, 0x40($sp)" :: "f"(r) : "memory");
    register float hlen __asm__("$f12") = halfLength;
    register float negH __asm__("$f14") = -hlen;
    register float zero __asm__("$f15") = 0.0f;

    int x0;
    int y0;
    int z0;
    int x1;
    int y1;
    int z1;
    __asm__ volatile("mfc1 %0, %1" : "=r"(x0) : "f"(zero));
    __asm__ volatile("mfc1 %0, %1" : "=r"(y0) : "f"(zero));
    __asm__ volatile("mfc1 %0, %1" : "=r"(z0) : "f"(negH));
    __asm__ volatile(
        "mtv %0, S120\n"
        "mtv %1, S121\n"
        "mtv %2, S122\n"
        "sv.q C120, 0x20($sp)\n"
        :: "r"(x0), "r"(y0), "r"(z0) : "memory");

    __asm__ volatile("mfc1 %0, %1" : "=r"(x1) : "f"(zero));
    __asm__ volatile("mfc1 %0, %1" : "=r"(y1) : "f"(zero));
    __asm__ volatile("mfc1 %0, %1" : "=r"(z1) : "f"(hlen));
    __asm__ volatile(
        "mtv %0, S130\n"
        "mtv %1, S131\n"
        "mtv %2, S132\n"
        "sv.q C130, 0x30($sp)\n"
        "vmov.q C200, C120\n"
        "vsub.t C120, C130, C200\n"
        "vdot.t S100, C120, C120\n"
        "vcmp.s ez, S100\n"
        "vrsq.s S101, S100\n"
        "vsqrt.s S102, S100\n"
        "vpfxs 1, Y, Z, W\n"
        "vcmovt.s S101, S100, 0\n"
        "vscl.t C120, C120, S101\n"
        "sv.q C120, 0x50($sp)\n"
        :: "r"(x1), "r"(y1), "r"(z1) : "memory");

    int distBits;
    __asm__ volatile("mfv %0, S102" : "=r"(distBits));
    float dist;
    __asm__ volatile("mtc1 %1, %0" : "=f"(dist) : "r"(distBits));

    float diameter = r * 2.0f;
    float sum = dist + diameter;
    float denom = diameter * 0.6f;
    int count = (int)(sum / denom);
    if (count < 3) {
        int minCount = 2;
        __asm__ volatile("" : "+r"(minCount));
        count = minCount;
        *(int *)((char *)this + 0x88) = count;
    } else {
        if (count >= 4) {
            count = 4;
        }
        *(int *)((char *)this + 0x88) = count;
    }

    __asm__ volatile("" : "+r"(count));
    float step = dist / (float)(count - 1);
    int i = 0;
    if (i < count) {
        __asm__ volatile("lv.q C130, 0x50($sp)" ::: "memory");
        char *dst = (char *)this + 0x90;
        do {
            float scale = step * (float)i;
            int scaleBits;
            __asm__ volatile("mfc1 %0, %1" : "=r"(scaleBits) : "f"(scale));
            __asm__ volatile(
                "mtv %0, S100\n"
                "vscl.t C120, C130, S100\n"
                "vadd.t C120, C200, C120\n"
                "sv.q C120, 0(%1)\n"
                :: "r"(scaleBits), "r"(dst) : "memory");
            *(float *)(dst + 0x0C) = r;
            i++;
            dst += 0x10;
        } while (i < *(int *)((char *)this + 0x88));
        __asm__ volatile("sv.q C120, 0x60($sp)" ::: "memory");
    }

    __0oKcReadBlockdtv(rb, 2);
    return result;
}

#pragma control sched=2
