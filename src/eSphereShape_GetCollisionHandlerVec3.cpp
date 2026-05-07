// ODR-WARNING: this TU intentionally redeclares eSphereShape with only the
// GetCollisionHandler overload needed here. Do not include eSphereShape.h; the
// shared class declaration omits this transition-zone helper and changing it
// can perturb already-matched eSphereShape methods.
class mVec3;

typedef unsigned int SceULong128 __attribute__((mode(TI)));

class eSphereShape {
public:
    char _pad[0x74];
    float mRadius;

    int GetCollisionHandler(const mVec3 &) const;
};

extern int eSphereShape_vec_collision_handler_guard asm("D_0000FA28");
extern struct eSphereShape_VecCollisionHandlerStorage
    eSphereShape_vec_collision_handler_storage asm("D_0000FA38");

struct eSphereShape_VecCollisionHandlerStorage {
    char pad00[0x30];
    int field30;
    char pad34[0x8];
    void *vtable;
    char pad40[0x60];
    char arrayA0[0x4820];
    union {
        SceULong128 sphere;
        struct {
            char pad[0xC];
            float radius;
        };
    } world;
    union {
        SceULong128 sphere;
        struct {
            char pad[0xC];
            float radius;
        };
    } local;
};

extern "C" void *__vec_new(void *, int, int, void *);

#pragma control sched=1
int eSphereShape::GetCollisionHandler(const mVec3 &pos) const {
    eSphereShape_VecCollisionHandlerStorage *handler =
        &eSphereShape_vec_collision_handler_storage;

    if (eSphereShape_vec_collision_handler_guard == 0) {
        eSphereShape_vec_collision_handler_guard = 1;
        handler->field30 = 0;
        handler->vtable = (void *)0x383110;
        char *array = handler->arrayA0;
        __asm__ volatile("" : "+r"(array));
        void *ctor = (void *)0x201BC8;
        __asm__ volatile("" : "+r"(ctor));
        __vec_new(array, 0x80, 0x90, ctor);
        handler->vtable = (void *)0x383130;
    }

    float radius = mRadius;
    volatile SceULong128 sphere;
    __asm__ volatile(
        "lv.q C120, 0(%0)\n"
        "sv.q C120, 0($sp)\n"
        :
        : "r"(&pos)
        : "memory"
    );
    *(volatile float *)((char *)&sphere + 0xC) = radius;
    __asm__ volatile(
        "lv.q C120, 0($sp)\n"
        "sv.q C120, 0x48c0(%0)\n"
        :
        : "r"(handler)
        : "memory"
    );
    handler->world.radius = radius;
    __asm__ volatile(
        "sv.q C120, 0x48d0(%0)\n"
        :
        : "r"(handler)
        : "memory"
    );
    handler->local.radius = radius;
    return (int)handler;
}
#pragma control sched=2
