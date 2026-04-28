#include "eBipedController.h"

class cBase;
class cFile;
class cMemPool;

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class ePhysicsController {
public:
    int Read(cFile &, cMemPool *);
};

void cFile_SetCurrentPos(void *, unsigned int);

extern "C" {
    void eBipedController__eBipedController_cBaseptr(void *self, cBase *parent);
}

struct eBipedControllerAllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

#pragma control sched=1

cBase *eBipedController::New(cMemPool *pool, cBase *parent) {
    eBipedController *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    eBipedControllerAllocRec *rec = (eBipedControllerAllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eBipedController *obj = (eBipedController *)rec->fn(base, 0x2C0, 0x10, 0, 0);
    if (obj != 0) {
        eBipedController__eBipedController_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

int eBipedController::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 && ((ePhysicsController *)this)->Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

void eBipedController::ApplyImpulse(int, const mVec3 &impulse, const mVec3 &) {
    void **entry = (void **)((char *)bodyEntries + (bodyIndex * 8));
    int ok = 0;
    if (entry[0] != 0) {
        if (entry[1] != 0) {
            ok = 1;
        }
    }
    ok = ok & 0xFF;
    if (ok != 0) {
        void *body = entry[0];
        __asm__ volatile(
            "lv.q C120, 0x40(%0)\n"
            "lv.q C130, 0x0(%1)\n"
            "vadd.t C120, C120, C130\n"
            "sv.q C120, 0x40(%0)\n"
            :: "r"(body), "r"(&impulse)
        );
    }
}

#pragma control sched=2

void eBipedController::InvalidateCacheEntries(eGeom *geom) {
    if (geom == *(eGeom **)((char *)this + 0x44)) {
        *(int *)((char *)this + 0x44) = 0;
        *(int *)((char *)this + 0x48) = 0;
    }
}

void eBipedController::EnableWallWalk(bool enable) {
    *(unsigned char *)((char *)this + 0xE0) = enable;
    if (!enable) {
        int a, b, c;
        float z = 0.0f;
        __asm__ volatile("mfc1 %0, %1" : "=r"(a) : "f"(z));
        __asm__ volatile("mfc1 %0, %1" : "=r"(b) : "f"(z));
        float neg = -1.0f;
        __asm__ volatile("mfc1 %0, %1" : "=r"(c) : "f"(neg));
        __asm__ volatile(
            "mtv %0, S120\n"
            "mtv %1, S121\n"
            "mtv %2, S122\n"
            "sv.q C120, 0x20(%3)\n"
            :: "r"(a), "r"(b), "r"(c), "r"(this)
        );
    }
}

void eBipedController::CollectContactCallBack(eContactCollector *coll, void *biped, eContact *contact, int idx) {
    if (biped != 0) {
        ((eBipedController *)biped)->CollectContact(coll, contact, idx);
    }
}

void eBipedController::ProcessContactCallBack(eContactCollector *coll, void *biped, eContact *contact, int idx) {
    if (biped != 0) {
        ((eBipedController *)biped)->ProcessContact(coll, contact, idx);
    }
}

void eBipedController::ProcessGroundOnlyContactCallBack(eContactCollector *coll, void *biped, eContact *contact, int idx) {
    if (biped != 0) {
        ((eBipedController *)biped)->ProcessGroundOnlyContact(coll, contact, idx);
    }
}
