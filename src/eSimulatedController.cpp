#include "eSimulatedController.h"
#include "eBipedController.h"
#include "cFile.h"

#pragma control sched=1

class cMemPool;
class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class eSimulatedControllerConfig {
public:
    const cType *GetType(void) const;
};

struct eSimulatedBodyEntry {
    char _pad[0x20];
    void *body;
    void *cache;
};

class eSimulatedState {
public:
    void WakeUp(void);
};

extern cType *D_000385DC;
extern cType *D_000469D8;
extern cType *D_000469E8;
extern cType *D_00046BB4;
extern cType *D_00046BD4;
extern cType *D_00046BF8;
extern "C" void *__vec_new(void *, int, int, void *);
extern "C" void *eSimulatedContact__eSimulatedContact_void__00209D24(void *);
void cFile_SetCurrentPos(void *, unsigned int);

typedef int v4sf_t __attribute__((mode(V4SF)));

class eRigidBodyState {
public:
    char _pad0[0x30];
    volatile v4sf_t position;
    char _pad1[0x68];
    unsigned int collisionMask;

    void Update(void);
    void UpdatePairs(void);
};

#pragma control sched=2
eSimulatedController::eSimulatedController(cBase *parent)
    : ePhysicsController(parent)
{
    *(void **)((char *)this + 4) = (void *)0x3834D8;
    *(unsigned char *)((char *)this + 0x10) = 0;
    *(unsigned char *)((char *)this + 0x11) = 0;
    *(unsigned char *)((char *)this + 0x12) = 0;
    __asm__ volatile(
        "mtc1 $0, $f12\n"
        "mfc1 $4, $f12\n"
        "mfc1 $5, $f12\n"
        "lui $6, 0xbf80\n"
        "mtc1 $6, $f13\n"
        "mfc1 $6, $f13\n"
        "mtv $4, S120\n"
        "mtv $5, S121\n"
        "mtv $6, S122\n"
        "sv.q C120, 0x20(%0)\n"
        :
        : "r"(this)
        : "$4", "$5", "$6", "$f12", "$f13", "memory");
    *(float *)((char *)this + 0x30) = 1.0f;
    *(int *)((char *)this + 0x38) = 0;
    *(int *)((char *)this + 0x3C) = 0;
    *(void **)((char *)this + 0x40) = this;
    *(int *)((char *)this + 0x44) = 0;
    *(void **)((char *)this + 0x48) = this;
    __vec_new((char *)this + 0x50, 4, 0x40,
              (void *)eSimulatedContact__eSimulatedContact_void__00209D24);
    *(int *)((char *)this + 0x150) = 0;
    *(int *)((char *)this + 0x154) = 0;
    *(int *)((char *)this + 0x158) = 0;
}

#pragma control sched=1
int eSimulatedController::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 &&
        ((ePhysicsController *)this)->Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

void eSimulatedController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const ePhysicsController *)this)->Write(file);
    wb.End();
}

bool eSimulatedController::IsSleeping(void) const {
    int i;
    __asm__ volatile("ori %0, $0, 0" : "=r"(i));
    char *entries = (char *)bodyEntries;
    int mask = 0x3FFFFFFF;
    int offset;
    __asm__ volatile("ori %0, $0, 0" : "=r"(offset));
    do {
        int count = 0;
        if (entries != 0) {
            count = *(int *)(entries - 4) & mask;
        }
        if (i < count) {
            eSimulatedBodyEntry *entry = (eSimulatedBodyEntry *)(entries + offset);
            void *body = entry->body;
            i++;
            if ((*(unsigned short *)((char *)body + 0x98) & 0x40) == 0) {
                return false;
            }
            offset += 0x30;
        } else {
            return true;
        }
    } while (true);
}

bool eSimulatedController::IsInFluid(void) const {
    bool result = false;
    int i;
    __asm__ volatile("ori %0, $0, 0" : "=r"(i));
    char *entries = (char *)bodyEntries;
    int mask = 0x3FFFFFFF;
    int offset;
    __asm__ volatile("ori %0, $0, 0" : "=r"(offset));
    do {
        int count = 0;
        if (entries != 0) {
            count = *(int *)(entries - 4) & mask;
        }
        if (i < count) {
            eSimulatedBodyEntry *entry = (eSimulatedBodyEntry *)(entries + offset);
            void *body = entry->body;
            int inFluid = (*(unsigned short *)((char *)body + 0x98) & 0x10) != 0;
            inFluid &= 0xFF;
            result = (bool)(result | inFluid);
            i++;
            offset += 0x30;
        } else {
            return result;
        }
    } while (true);
}

void eSimulatedController::SetPosition(int index, const mVec3 &position) {
    __asm__ volatile("" : : "r"(&position) : "$6");
    char *entries = (char *)bodyEntries;
    eSimulatedBodyEntry *entry =
        (eSimulatedBodyEntry *)(entries + (index * 0x30));
    int ok = 0;
    if (entry->body != 0) {
        if (entry->cache != 0) {
            ok = 1;
        }
    }
    ok = ok & 0xFF;
    if (ok != 0) {
        eRigidBodyState *body = (eRigidBodyState *)entry->body;
        body->position = *(const v4sf_t *)&position;
        body->Update();
    }
}

void eSimulatedController::ApplyTorque(int index, const mVec3 &torque) {
    char *entries = (char *)bodyEntries;
    __asm__ volatile("" : "+r"(entries));
    eSimulatedBodyEntry *entry =
        (eSimulatedBodyEntry *)(entries + (index * 0x30));
    int ok = 0;
    if (entry->body != 0) {
        if (entry->cache != 0) {
            ok = 1;
        }
    }
    ok = ok & 0xFF;
    if (ok != 0) {
        void *body = entry->body;
        __asm__ volatile(
            "lv.q C120, 0x10(%0)\n"
            "lv.q C130, 0(%1)\n"
            "vadd.t C120, C120, C130\n"
            "sv.q C120, 0x10(%0)\n"
            :
            : "r"(entry), "r"(&torque)
            : "memory");
        if ((*(unsigned short *)((char *)body + 0x98) & 0x40) != 0) {
            ((eSimulatedState *)entry)->WakeUp();
        }
    }
}

void eSimulatedController::ClearExternalForces(void) {
    char *entries = (char *)bodyEntries;
    int count = 0;
    if (entries != 0) {
        count = *(int *)(entries - 4) & 0x3FFFFFFF;
    }
    int i = 0;
    if (i < count) {
        __asm__ volatile("mtc1 $0, $f12" ::: "$f12");
        do {
            __asm__ volatile(
                "mfc1 $7, $f12\n"
                "mfc1 $8, $f12\n"
                "mfc1 $9, $f12\n"
                "mtv $7, S120\n"
                "mtv $8, S121\n"
                "mtv $9, S122\n"
                "sv.q C120, 0(%0)\n"
                "mfc1 $7, $f12\n"
                "mfc1 $8, $f12\n"
                "mfc1 $9, $f12\n"
                "mtv $7, S120\n"
                "mtv $8, S121\n"
                "mtv $9, S122\n"
                "sv.q C120, 0x10(%0)\n"
                :
                : "r"(entries)
                : "$7", "$8", "$9", "memory");
            i++;
            entries += 0x30;
        } while (i < count);
    }
}

void eSimulatedController::SetCollisionMask(unsigned int mask) {
    collisionMask = mask;
    int i = 0;
    int arrayMask = 0x3FFFFFFF;
    __asm__ volatile("" : "+r"(arrayMask));
    register char *entries __asm__("$4");
    __asm__ volatile("lw %0, 0x38(%1)" : "=r"(entries) : "r"(this));
    int offset = 0;
    do {
        int count = 0;
        if (entries != 0) {
            count = *(int *)(entries - 4) & arrayMask;
        }
        if (i < count) {
            __asm__ volatile("" ::: "memory");
            unsigned int currentMask = collisionMask;
            eSimulatedBodyEntry *entry =
                (eSimulatedBodyEntry *)(entries + offset);
            ((eRigidBodyState *)entry->body)->collisionMask = currentMask;
            ((eRigidBodyState *)
                 ((eSimulatedBodyEntry *)((char *)bodyEntries + offset))->body)
                ->UpdatePairs();
            i++;
            offset += 0x30;
            entries = (char *)bodyEntries;
        } else {
            return;
        }
    } while (true);
}

const cType *eBipedController::GetType(void) const {
    if (D_00046BB4 == 0) {
        if (D_000469D8 == 0) {
            if (D_000385DC == 0) {
                const char *name = (const char *)0x36CD74;
                const char *desc = (const char *)0x36CD7C;
                __asm__ volatile("" : "+r"(name), "+r"(desc));
                D_000385DC = cType::InitializeType(
                    name, desc, 1, 0, 0, 0, 0, 0);
            }
            D_000469D8 = cType::InitializeType(
                0, 0, 0x232, D_000385DC, 0, 0, 0, 0);
        }
        __asm__ volatile("" ::: "memory");
        const cType *parentType = D_000469D8;
        cBase *(*factory)(cMemPool *, cBase *) =
            (cBase *(*)(cMemPool *, cBase *))0x208DDC;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046BB4 = cType::InitializeType(
            0, 0, 0x238, parentType, factory, 0, 0, 0);
    }
    return D_00046BB4;
}

const cType *eSimulatedController::GetType(void) const {
    if (D_00046BD4 == 0) {
        if (D_000469D8 == 0) {
            if (D_000385DC == 0) {
                const char *name = (const char *)0x36CD74;
                const char *desc = (const char *)0x36CD7C;
                __asm__ volatile("" : "+r"(name), "+r"(desc));
                D_000385DC = cType::InitializeType(
                    name, desc, 1, 0, 0, 0, 0, 0);
            }
            D_000469D8 = cType::InitializeType(
                0, 0, 0x232, D_000385DC, 0, 0, 0, 0);
        }
        D_00046BD4 = cType::InitializeType(
            0, 0, 0x233, D_000469D8, 0, 0, 0, 0);
    }
    return D_00046BD4;
}

const cType *eSimulatedControllerConfig::GetType(void) const {
    if (D_00046BF8 == 0) {
        if (D_000469E8 == 0) {
            if (D_000385DC == 0) {
                const char *name = (const char *)0x36CD74;
                const char *desc = (const char *)0x36CD7C;
                __asm__ volatile("" : "+r"(name), "+r"(desc));
                D_000385DC = cType::InitializeType(
                    name, desc, 1, 0, 0, 0, 0, 0);
            }
            D_000469E8 = cType::InitializeType(
                0, 0, 0x23D, D_000385DC, 0, 0, 0, 0);
        }
        D_00046BF8 = cType::InitializeType(
            0, 0, 0x23E, D_000469E8, 0, 0, 0, 0);
    }
    return D_00046BF8;
}
