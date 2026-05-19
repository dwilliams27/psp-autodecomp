// gcUIGeom_Cull.cpp - split TU for Cull.
// ODR-WARNING: gcUIGeom is also defined in src/gcUIGeom.cpp. This TU
// redeclares only the Cull method so existing matched methods keep their
// current codegen.

class cBase;
class cFile;
class cMemPool;
class cType;
class eGeom;
class eDynamicGeom;
class eCamera;
class mFrustum;
class eCameraBins;
class eMaterial;
template <class T> class cHandleT;

class cTimeValue {
public:
    int mTime;
};

class eGeom {
public:
    int base;
};

class eDynamicGeom : public eGeom {
public:
    eDynamicGeom(cBase *);
    ~eDynamicGeom();
};

class gcUIGeom : public eDynamicGeom {
public:
    void Cull(unsigned int, const eCamera &, const mFrustum &, eCameraBins *,
              unsigned int, int, const cHandleT<eMaterial> *, float) const;
};

extern "C" float fabsf(float);

struct VtSlot {
    short offset;
    short _pad;
    void (*fn)(void *);
};

struct CullEntry {
    unsigned short flags;       // +0xC of real entry
    unsigned short field_E;      // +0xE
    unsigned short field_10;     // +0x10
    unsigned char binId;          // +0x12
    unsigned char alpha;          // +0x13
    int field_14;                  // +0x14
    int field_18;                  // +0x18
    void *thisPtr;                 // +0x1C
    void *cameraPtr;               // +0x20
    float fade;                    // +0x24
    short binCountAtAdd;           // +0x28
    short distScaled;              // +0x2A
};

// ── gcUIGeom::Cull(...) const @ 0x0013b4e4 ──
void gcUIGeom::Cull(unsigned int stamp, const eCamera &camera,
                    const mFrustum & /*frustum*/, eCameraBins *bins,
                    unsigned int /*arg5*/, int arg6,
                    const cHandleT<eMaterial> *arg7, float fade) const {
    if (stamp == *(const unsigned int *)((const char *)this + 0x88)) return;
    *(unsigned int *)((char *)this + 0x88) = stamp;
    if (*(const float *)((const char *)this + 0xD4) == 0.0f) return;
    if ((arg6 | (int)arg7) != 0) return;

    void *mesh = *(void **)((const char *)this + 0x60);
    if (mesh == 0) return;

    if (*(const unsigned char *)((const char *)this + 0x8C) & 4) {
        char *vt = *(char **)((const char *)this + 4);
        VtSlot *slot = (VtSlot *)(vt + 0xB8);
        slot->fn((char *)this + slot->offset);
    }

    const char *camBase = (const char *)&camera + 0x20;
    __asm__("" : "+r"(camBase));
    const char *p40 = camBase + 0x20;
    const char *p50 = camBase + 0x30;
    int dotBits;
    __asm__ volatile(
        "lv.q C120, 0(%1)\n"
        "lv.q C130, 0(%2)\n"
        "vdot.t S100, C120, C130\n"
        "mfv %0, S100\n"
        : "=r"(dotBits) : "r"(p40), "r"(p50) : "memory"
    );
    float dot = *(float *)&dotBits;

    unsigned char nearMinusOne = 0;
    if (fabsf(-1.0f - dot) <= 9.99999975e-06f) {
        nearMinusOne = 1;
    }

    float dist;
    if ((nearMinusOne & 0xFF) != 0) {
        dist = 0.0f;
    } else {
        int dlenBits;
        __asm__ volatile(
            "lv.q C120, 0x110(%1)\n"
            "lv.q C130, 0x40(%2)\n"
            "vsub.t C120, C120, C130\n"
            "vdot.t S100, C120, C120\n"
            "vsqrt.s S100, S100\n"
            "mfv %0, S100\n"
            : "=r"(dlenBits) : "r"(&camera), "r"(this) : "memory"
        );
        float dlen = *(float *)&dlenBits;
        dist = dlen * *(const float *)((const char *)&camera + 0x1E4);
    }

    float threshold = *(const float *)((const char *)mesh + 0x44) + 1.0f;
    float alpha;
    if (dist <= threshold) {
        alpha = 1.0f;
    } else {
        alpha = 1.0f - (dist - threshold) * 0.5f;
        if (alpha < 0.0f) alpha = 0.0f;
    }
    if (alpha == 0.0f) return;

    fade *= *(const float *)((const char *)this + 0xD4);
    if ((*(const unsigned short *)((const char *)bins + 4) & 0x7F) == 0) return;
    fade *= alpha;

    if (*(const int *)((const char *)bins + 8) >= 3000) return;

    int *countPtr = (int *)((char *)bins + 8);
    int count = *countPtr;
    CullEntry *entry = (CullEntry *)((char *)bins + count * 32 + 0xC);
    *countPtr = count + 1;

    entry->flags = 0x80;

    int binIdx = 2;
    if (*(const unsigned char *)((const char *)bins + 0x1770C)) {
        binIdx = *(const int *)((const char *)bins + 0x17710);
    }
    entry->flags = (unsigned short)(entry->flags | (binIdx << 9));

    unsigned int sub = *(const unsigned char *)((const char *)bins + 0x18400);
    int *counter = (int *)((char *)bins + 0x18374 + binIdx * 20 + sub * 4);
    *counter = *counter + 1;

    entry->field_18 = 0;
    if (fade < 1.0f) {
        entry->flags = (unsigned short)(entry->flags | 0x10);
    }
    entry->field_E = 0;
    entry->field_14 = 0;
    entry->field_10 = 0;
    entry->thisPtr = (void *)this;
    entry->cameraPtr = (void *)&camera;
    entry->fade = fade;
    entry->binId = (unsigned char)sub;
    entry->alpha = 0xFF;
    entry->distScaled = (short)(int)(dist * 5.0f);
    entry->binCountAtAdd = (short)*countPtr;
}
