#include "mOCS.h"
#include "mVec3.h"
#include "cBase.h"

typedef unsigned int SceULong128 __attribute__((mode(TI)));

struct eContactCollector {};

class eCollisionHandler {
public:
    void ProcessAllCollisions(void);
};

class eEmbedVolumeCollisionHandler : public eCollisionHandler {
public:
    void SetLocalToWorld(const mOCS &);
    void GetResolveVector(mVec3 *);
    unsigned char Run(void);
};

void eContactCollector__ClearAllContacts_void(eContactCollector *);

// 0x00069d90, 36B: copies mOCS into this+0x60 (4 quadwords).
void eEmbedVolumeCollisionHandler::SetLocalToWorld(const mOCS &ocs) {
    *(SceULong128 *)((char *)this + 0x90) = *(const SceULong128 *)((const char *)&ocs + 0x30);
    *(SceULong128 *)((char *)this + 0x60) = *(const SceULong128 *)((const char *)&ocs + 0x00);
    *(SceULong128 *)((char *)this + 0x70) = *(const SceULong128 *)((const char *)&ocs + 0x10);
    *(SceULong128 *)((char *)this + 0x80) = *(const SceULong128 *)((const char *)&ocs + 0x20);
}

// 0x00069db4, 120B: accumulates scaled contact vectors into *out.
struct eEmbedContact {
    SceULong128 qw;       // +0x00
    int _pad_10[4];       // +0x10
    float scale;          // +0x20
    int _pad_24[27];      // +0x24..+0x8c
};

void eEmbedVolumeCollisionHandler::GetResolveVector(mVec3 *out) {
    float z = 0.0f;
    int a, b, c;
    __asm__ volatile("mfc1 %0, %1" : "=r"(a) : "f"(z));
    __asm__ volatile("mfc1 %0, %1" : "=r"(b) : "f"(z));
    __asm__ volatile("mfc1 %0, %1" : "=r"(c) : "f"(z));
    __asm__ volatile(
        "mtv %0, S120\n"
        "mtv %1, S121\n"
        "mtv %2, S122\n"
        "sv.q C120, 0(%3)\n"
        :: "r"(a), "r"(b), "r"(c), "r"(out) : "memory"
    );

    eEmbedContact *arr = (eEmbedContact *)((char *)this + 0x12a0);
    int i = 0;
    int *cnt = (int *)((char *)this + 0x48a0);
    if (i < cnt[1]) {
        do {
            float s = arr->scale;
            int t;
            __asm__ volatile("mfc1 %0, %1" : "=r"(t) : "f"(s));
            __asm__ volatile(
                "mtv %0, S100\n"
                "lv.q C120, 0(%1)\n"
                "lv.q C130, 0(%2)\n"
                "vscl.t C120, C120, S100\n"
                "vadd.t C120, C130, C120\n"
                "sv.q C120, 0(%2)\n"
                :: "r"(t), "r"(arr), "r"(out) : "memory"
            );
            i++;
            arr++;
        } while (i < cnt[1]);
    }
}

// 0x00069cdc, 180B: Main Run entry — clears contacts, processes collisions,
// dispatches callbacks for first and second contact lists.
#pragma control sched=1
unsigned char eEmbedVolumeCollisionHandler::Run(void) {
    eContactCollector *cc = (eContactCollector *)((char *)this + 0x40);
    eContactCollector__ClearAllContacts_void(cc);
    this->ProcessAllCollisions();
    int *counts = (int *)((char *)this + 0x48a0);
    typedef void (*Cb)(eContactCollector *, int, void *, int);
    Cb cb = *(Cb *)((char *)this + 0x48b0);
    if (counts[1] > 0) {
        *(unsigned char *)((char *)this + 0x48b8) = 1;
    }
    if (cb != 0) {
        int n1 = counts[1];
        if (n1 > 0) {
            int arg1 = *(int *)((char *)this + 0x48b4);
            cb(cc, arg1, (char *)this + 0x12a0, n1);
            cb = *(Cb *)((char *)this + 0x48b0);
        }
        if (cb != 0) {
            int n2 = counts[2];
            if (n2 > 0) {
                int arg1 = *(int *)((char *)this + 0x48b4);
                cb(0, arg1, (char *)this + 0x24a0, n2);
            }
        }
    }
    return *(unsigned char *)((char *)this + 0x48b8);
}
#pragma control sched=2

// ── eSimulatedMotor ──────────────────────────────────────────────────────
extern char eSimulatedMotorclassdesc[];

class eSimulatedMotor {
public:
    eSimulatedMotor(cBase *);
    cBase *base;
    void *vtable;
    int pad[3];
};

// 0x0006b694, 36B: constructor.
eSimulatedMotor::eSimulatedMotor(cBase *parent) {
    base = parent;
    vtable = eSimulatedMotorclassdesc;
    pad[0] = 0;
    pad[1] = 0;
    pad[2] = 0;
}

// ── eConvexHullUtil ──────────────────────────────────────────────────────
class eConvexHullUtil {
public:
    float v[7];
    void Begin(void);
};

// 0x0006e3e8, 36B: zero-init 7 floats.
void eConvexHullUtil::Begin(void) {
    v[0] = 0.0f;
    v[1] = 0.0f;
    v[2] = 0.0f;
    v[3] = 0.0f;
    v[4] = 0.0f;
    v[5] = 0.0f;
    v[6] = 0.0f;
}
