#pragma control sched=1
// Split TU for eShadow::Cull.
// ODR-WARNING: eShadow is also defined in src/eShadow.cpp. This TU redeclares
// only the Cull method so existing matched methods keep their current codegen.

class cBase;
class cMemPool;
class cType;
class eCamera;
class mFrustum;
class eCameraBins;
class eMaterial;
template <class T> class cHandleT;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class eDynamicModel {
public:
    static cBase *New(cMemPool *, cBase *);
};

class eShadow {
public:
    void Cull(unsigned int, const eCamera &, const mFrustum &, eCameraBins *,
              unsigned int, int, const cHandleT<eMaterial> *, float) const;
};

extern "C" float fabsf(float);

extern cType *D_000385DC;
extern cType *D_00040FF4;
extern cType *D_000469C0;
extern cType *D_000469DC;

struct TypeSlot {
    short offset;
    short pad;
    cType *(*fn)(void *);
};

struct VoidSlot {
    short offset;
    short pad;
    void (*fn)(void *);
};

struct OpacitySlot {
    short offset;
    short pad;
    float (*fn)(void *, const eCamera &, float);
};

struct TypeNode {
    char pad[0x1C];
    cType *parent;
};

struct CullEntry {
    unsigned short flags;
    unsigned short field_E;
    unsigned short field_10;
    unsigned char binId;
    unsigned char alpha;
    int field_14;
    int field_18;
    const void *thisPtr;
    const eCamera *cameraPtr;
    float fade;
    short binCountAtAdd;
    short distScaled;
};

void eShadow::Cull(unsigned int stamp, const eCamera &camera,
                   const mFrustum & /*frustum*/, eCameraBins *bins,
                   unsigned int /*arg5*/, int arg6,
                   const cHandleT<eMaterial> *arg7, float fade) const {
    if ((*(unsigned int *)0x37D0F0 & 8) == 0) {
        return;
    }
    if ((*(unsigned short *)((char *)bins + 4) & 8) != 0) {
        return;
    }
    if (stamp == *(unsigned int *)((const char *)this + 0x88)) {
        return;
    }
    *(unsigned int *)((char *)this + 0x88) = stamp;
    __asm__ volatile("" ::: "memory");

    float opacity = *(const float *)((const char *)this + 0xD4);
    float dist = 0.0f;
    if (opacity == 0.0f) {
        return;
    }
    if ((arg6 | (int)arg7) != 0) {
        return;
    }

    unsigned char *parent = *(unsigned char **)((const char *)this + 0xDC);
    if (parent == 0) {
        return;
    }
    if ((*(unsigned char *)(parent + 0xD2) & 0x20) != 0) {
        return;
    }

    unsigned char *model = *(unsigned char **)((char *)parent + 0xDC);
    unsigned char *dynamicModel = 0;
    if (model != 0) {
        if (D_000469DC == 0) {
            if (D_000469C0 == 0) {
                if (D_00040FF4 == 0) {
                    if (D_000385DC == 0) {
                        D_000385DC = cType::InitializeType(
                            (const char *)0x36CD74, (const char *)0x36CD7C, 1,
                            0, 0, 0, 0, 0);
                    }
                    D_00040FF4 = cType::InitializeType(
                        0, 0, 0x16, D_000385DC, 0, 0, 0, 0);
                }
                D_000469C0 = cType::InitializeType(
                    0, 0, 0x17, D_00040FF4, 0, 0, 0, 0);
            }
            D_000469DC = cType::InitializeType(
                0, 0, 0x2D, D_000469C0, eDynamicModel::New, 0, 0, 0);
        }

        cType *target = D_000469DC;
        TypeSlot *slot = (TypeSlot *)(*(char **)(model + 4) + 8);
        cType *type = slot->fn((char *)model + slot->offset);
        int isModel;
        if (target == 0) {
            isModel = 0;
            goto type_done;
        }
        if (type != 0) {
type_loop:
            if (type == target) {
                isModel = 1;
                goto type_done;
            }
            type = ((TypeNode *)type)->parent;
            if (type != 0) {
                goto type_loop;
            } else {
                isModel = 0;
                }
        } else {
            isModel = 0;
        }
type_done:
        if (isModel != 0) {
            dynamicModel = model;
        }
    }

    if (dynamicModel != 0) {
        unsigned int hidden =
            (*(unsigned char *)(dynamicModel + 0xD2) & 0x20) != 0;
        if (hidden == 0) {
            return;
        }
    }

    if (*(int *)((const char *)this + 0x60) == 0) {
        return;
    }

    {
        char *vt = *(char **)((const char *)this + 4);
        OpacitySlot *slot = (OpacitySlot *)(vt + 0xD8);
        short off = slot->offset;
        void *base = (char *)this + off;
        float (*fn)(void *, const eCamera &, float) = slot->fn;
        opacity = fn(base, camera, fade);
    }
    if (opacity == 0.0f) {
        return;
    }

    if ((*(unsigned char *)((const char *)this + 0x8C) & 4) != 0) {
        char *vt = *(char **)((const char *)this + 4);
        VoidSlot *slot = (VoidSlot *)(vt + 0xB8);
        short off = slot->offset;
        void (*fn)(void *) = slot->fn;
        fn((char *)this + off);
    }

    const char *camBase = (const char *)&camera + 0x20;
    const char *p40 = camBase + 0x20;
    const char *p50 = camBase + 0x30;
    int dotBits;
    __asm__ volatile(
        "lv.q C120, 0(%1)\n"
        "lv.q C130, 0(%2)\n"
        "vdot.t S100, C120, C130\n"
        "mfv %0, S100\n"
        : "=r"(dotBits) : "r"(p40), "r"(p50) : "memory");
    float dot = *(float *)&dotBits;

    unsigned int nearMinusOne = 0;
    if (fabsf(-1.0f - dot) <= 9.99999975e-06f) {
        nearMinusOne = 1;
    }
    if ((nearMinusOne & 0xFF) == 0) {
        int distBits;
        __asm__ volatile(
            "lv.q C120, 0x110(%1)\n"
            "lv.q C130, 0x40(%2)\n"
            "vsub.t C120, C120, C130\n"
            "vdot.t S100, C120, C120\n"
            "vsqrt.s S100, S100\n"
            "mfv %0, S100\n"
            : "=r"(distBits) : "r"(&camera), "r"(this) : "memory");
        dist = *(float *)&distBits;
    }

    CullEntry *entry;
    if ((*(unsigned short *)((char *)bins + 4) & 0x7F) != 0) {
        if (*(int *)((char *)bins + 8) < 3000) {
            int count = *(int *)((char *)bins + 8);
            *(int *)((char *)bins + 8) = count + 1;
            entry = (CullEntry *)((char *)bins + (count << 5) + 0xC);
            entry->flags = 0x20;

            int binIdx = 2;
            if (*(unsigned char *)((char *)bins + 0x1770C) != 0) {
                binIdx = *(int *)((char *)bins + 0x17710);
            }
            entry->flags = (unsigned short)(entry->flags | (binIdx << 9));

            unsigned int sub = *(unsigned char *)((char *)bins + 0x18400);
            int *counter =
                (int *)((char *)bins + 0x18374 + binIdx * 20 + sub * 4);
            *counter = *counter + 1;

            entry->field_18 = 0;
            if (opacity < 1.0f) {
                entry->flags = (unsigned short)(entry->flags | 0x10);
            }
            entry->field_E = 0;
            entry->field_14 = 0;
            entry->field_10 = 0;
            entry->thisPtr = this;
            entry->cameraPtr = &camera;
            entry->fade = opacity;
            entry->binId = *(unsigned char *)((char *)bins + 0x18400);
            entry->distScaled = (short)(int)(dist * 5.0f);
            entry->alpha = 0xFF;
            entry->binCountAtAdd = (short)*(int *)((char *)bins + 8);
        } else {
            entry = 0;
        }
    } else {
        entry = 0;
    }

    entry->flags = (unsigned short)(entry->flags | 0x600);
}
