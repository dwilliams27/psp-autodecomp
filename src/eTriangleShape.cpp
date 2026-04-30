// eTriangleShape.cpp - decompiled from eAll_psp.obj

class cBase;
class cMemPool;
class cType;
class mOCS;
class mVec3;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class eTriangleShape {
public:
    const cType *GetType(void) const;
    void GetProjectedMinMax(const mVec3 &, const mOCS &, float *, float *) const;
    void GetInertialTensor(float, mVec3 *) const;
    static cBase *New(cMemPool *, cBase *);
};

static cType *type_cBase;
static cType *type_eShape;
static cType *type_eTriangleShape;

#pragma control sched=1

// eTriangleShape::GetType(void) const - 0x0020ad14
const cType *eTriangleShape::GetType(void) const {
    if (!type_eTriangleShape) {
        if (!type_eShape) {
            if (!type_cBase) {
                const char *name = (const char *)0x36CD74;
                const char *desc = (const char *)0x36CD7C;
                __asm__ volatile("" : "+r"(name), "+r"(desc));
                type_cBase = cType::InitializeType(name, desc, 1, 0, 0, 0, 0, 0);
            }
            type_eShape = cType::InitializeType(0, 0, 0x227, type_cBase, 0, 0, 0, 0);
        }
        __asm__ volatile("" ::: "memory");
        const cType *parentType = type_eShape;
        cBase *(*factory)(cMemPool *, cBase *) =
            (cBase *(*)(cMemPool *, cBase *))0x20AC98;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        type_eTriangleShape =
            cType::InitializeType(0, 0, 0x2D5, parentType, factory, 0, 0, 0);
    }
    return type_eTriangleShape;
}

void eTriangleShape::GetInertialTensor(float mass, mVec3 *out) const {
    float scale = mass * 0.4f;
    float radius = *(const float *)((const char *)this + 0x74);
    float radiusSq = radius * radius;
    __asm__ volatile("mul.s %0, %0, %1" : "+f"(scale) : "f"(radiusSq));

    int x, y, z;
    __asm__ volatile("mfc1 %0, %1" : "=r"(x) : "f"(scale));
    __asm__ volatile("mfc1 %0, %1" : "=r"(y) : "f"(scale));
    __asm__ volatile("mfc1 %0, %1" : "=r"(z) : "f"(scale));
    __asm__ volatile(
        ".set push\n"
        ".set noreorder\n"
        "mtv %0, S120\n"
        "mtv %1, S121\n"
        "mtv %2, S122\n"
        "jr $ra\n"
        "sv.q C120, 0($a1)\n"
        "addu $zero, %3, $zero\n"
        ".set pop\n"
        :: "r"(x), "r"(y), "r"(z), "r"(out)
        : "memory"
    );
}

void eTriangleShape::GetProjectedMinMax(const mVec3 &dir, const mOCS &, float *outMin, float *outMax) const {
    const char *vertex = (const char *)this + 0x80;
    float dot;

    __asm__ volatile(
        "lv.q C120, 0(%1)\n"
        "lv.q C130, 0(%2)\n"
        "vdot.t S100, C120, C130\n"
        "mfv $a2, S100\n"
        "mtc1 $a2, %0\n"
        : "=f"(dot)
        : "r"(&dir), "r"(vertex)
        : "$a0", "$a2"
    );
    *outMin = dot;
    *outMax = dot;

    for (int i = 1; i < 3; i++) {
        vertex += 0x10;
        __asm__ volatile(
            "lv.q C120, 0(%1)\n"
            "lv.q C130, 0(%2)\n"
            "vdot.t S100, C130, C120\n"
            "mfv $t1, S100\n"
            "mtc1 $t1, %0\n"
            : "=f"(dot)
            : "r"(vertex), "r"(&dir)
            : "$a0", "$t1"
        );

        if (dot < *outMin) {
            *outMin = dot;
        }
        if (!(dot <= *outMax)) {
            *outMax = dot;
        }
    }
}

#pragma control sched=2
