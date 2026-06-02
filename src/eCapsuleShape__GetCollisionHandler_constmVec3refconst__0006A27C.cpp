// eCapsuleShape::GetCollisionHandler(const mVec3 &) const
// 0x0006a27c, 228B, eAll_psp.obj
// Split-TU: eCapsuleShape declared locally (transition-zone helper omitted
// from the shared header). ODR-WARNING: local class shape; do not include the
// real header here.
//
// Structurally identical to the matched sibling
// eMultiSphereShape::GetCollisionHandler(const mVec3 &) (0x00068d18); only the
// lazily-initialised handler guard/storage globals differ (0x222E8 / 0x222F8
// here). Builds the embedded collision handler, fills a stack mCapsule (two
// sphere centres displaced +/- mHalfLength along the local "up" axis C000,
// scaled radius) from the query position, then dispatches to
// eEmbedMultiSphereCollisionHandler::Set(const mCapsule &). The VFPU sequence
// has no scalar C equivalent (matrix column ops); the FPU/integer glue is
// expressed in C so the SNC scheduler interleaves it as in the original.

class mVec3;
class mCapsule;

typedef unsigned int SceULong128 __attribute__((mode(TI)));

class eCapsuleShape {
public:
    char _pad[0x80];
    float mRadius;       // 0x80
    float mHalfLength;   // 0x84

    int GetCollisionHandler(const mVec3 &) const;
};

extern int eCapsuleShape_vec_handler_guard asm("D_000222E8");
extern struct eCapsuleShape_VecHandlerStorage
    eCapsuleShape_vec_handler_storage asm("D_000222F8");

struct eCapsuleShape_VecHandlerStorage {
    char pad00[0x30];
    int field30;        // 0x30
    char pad34[0x8];
    void *vtable;       // 0x3C
    char pad40[0x60];
    char arrayA0[0x4820];
};

extern "C" void *__vec_new(void *, int, int, void *);
extern void eEmbedMultiSphereCollisionHandler__Set(void *handler,
                                                   const mCapsule &capsule)
    asm("__0fReEmbedMultiSphereCollisionHandlerTSetRC8FmCapsule");

#pragma control sched=1
int eCapsuleShape::GetCollisionHandler(const mVec3 &pos) const {
    eCapsuleShape_VecHandlerStorage *handler =
        &eCapsuleShape_vec_handler_storage;

    if (eCapsuleShape_vec_handler_guard == 0) {
        eCapsuleShape_vec_handler_guard = 1;
        handler->field30 = 0;
        handler->vtable = (void *)0x383110;
        char *array = handler->arrayA0;
        __asm__ volatile("" : "+r"(array));
        void *ctor = (void *)0x201BC8;
        __asm__ volatile("" : "+r"(ctor));
        __vec_new(array, 0x80, 0x90, ctor);
        handler->vtable = (void *)0x383150;
    }

    // Stack frame layout (mirrors original): a scratch quadword sits just below
    // the mCapsule, which holds sphere centre 0 at +0x00, sphere centre 1 at
    // +0x10, and the radius float at +0x20.
    SceULong128 scratch;
    struct LocalCapsule {
        SceULong128 sphere0;
        SceULong128 sphere1;
        float radius;
        char _pad[0xC];
    } capsule;

    float radius = mRadius;
    capsule.radius = radius;

    // up-vector (C000) selected through the [0,0,1,W] source prefix.
    __asm__ volatile(
        "vpfxs 0, 0, 1, W\n"
        "vmov.t C120, C000\n"
        :
        :
        : "memory"
    );

    float half = mHalfLength;
    float negHalf = -half;

    // base = &scratch (= sp+0x00); capsule lives at base+0x10.
    void *base = &scratch;

    // Centre 0: pos + (-half) * up; store at capsule sphere0 (base+0x10).
    __asm__ volatile(
        "mtv  %1, S100\n"
        "lv.q C130, 0(%2)\n"
        "vscl.t C120, C120, S100\n"
        "vadd.t C120, C130, C120\n"
        "sv.q C120, 0x10(%0)\n"
        :
        : "r"(base), "r"(negHalf), "r"(&pos)
        : "memory"
    );

    // Centre 1: pos + half * up; store at capsule sphere1 (base+0x20).
    __asm__ volatile(
        "vpfxs 0, 0, 1, W\n"
        "vmov.t C120, C000\n"
        "sv.q C120, 0(%0)\n"
        "mtv  %1, S100\n"
        "vscl.t C120, C120, S100\n"
        "vadd.t C120, C130, C120\n"
        "sv.q C120, 0x20(%0)\n"
        :
        : "r"(base), "r"(half)
        : "memory"
    );

    eEmbedMultiSphereCollisionHandler__Set(handler,
                                           *(const mCapsule *)&capsule);
    return (int)handler;
}
#pragma control sched=2
