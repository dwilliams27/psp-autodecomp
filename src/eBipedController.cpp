class cBase;
class cFile;
class cMemPool;
class cType;
class eGeom;
class eContact;
class eContactCollector;
class mVec3;

inline void *operator new(unsigned int, void *p) {
    return p;
}

typedef int v4sf_t __attribute__((mode(V4SF)));

union eBipedVec4 {
    v4sf_t v;
    struct {
        float x, y, z, w;
    } f;
};

struct eBipedMatrix {
    eBipedVec4 x;
    eBipedVec4 y;
    eBipedVec4 z;
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class ePhysicsController {
public:
    char _pad[0x10];
    ePhysicsController(cBase *);
    ~ePhysicsController(void);
    int Read(cFile &, cMemPool *);
};

void cFile_SetCurrentPos(void *, unsigned int);

struct eBipedControllerAllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

struct eBipedControllerDeleteRec {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class eBipedController : public ePhysicsController {
public:
    eBipedController(cBase *);
    ~eBipedController(void);

    static cBase *New(cMemPool *, cBase *);

    int Read(cFile &, cMemPool *);

    void ApplyImpulse(int, const mVec3 &, const mVec3 &);
    void ApplyForce(int, const mVec3 &, const mVec3 &);
    void ApplyPositionedImpulse(int, const mVec3 &, const mVec3 &);
    void ApplyPositionedForce(int, const mVec3 &, const mVec3 &);
    int GetCollisionMask(void) const;
    void OnSnappedTo(void);
    void GetVelocity(int, mVec3 *, mVec3 *) const;
    void InvalidateCacheEntries(eGeom *);
    void EnableWallWalk(bool);
    const cType *GetType(void) const;

    void CollectContact(eContactCollector *, eContact *, int);
    void ProcessContact(eContactCollector *, eContact *, int);
    void ProcessGroundOnlyContact(eContactCollector *, eContact *, int);

    static void CollectContactCallBack(eContactCollector *, void *, eContact *, int);
    static void ProcessContactCallBack(eContactCollector *, void *, eContact *, int);
    static void ProcessGroundOnlyContactCallBack(eContactCollector *, void *, eContact *, int);

    static void operator delete(void *p) {
        if (p != 0) {
            cMemPool *pool = cMemPool::GetPoolFromPtr(p);
            char *block = ((char **)pool)[9];
            eBipedControllerDeleteRec *rec =
                (eBipedControllerDeleteRec *)(((char **)block)[7] + 0x30);
            short off = rec->offset;
            __asm__ volatile("" ::: "memory");
            char *base = block + off;
            void (*fn)(void *, void *) = rec->fn;
            fn(base, p);
        }
    }

    int bodyIndex;          // 0x10
    char _pad1[0x1C];       // 0x14-0x2F
    void *bodyEntries;      // 0x30
    char _pad2[0xE0];       // 0x34-0x113
    int collisionMask;      // 0x114
};

extern char eBipedControllerclassdesc[];
extern int D_00038568;
extern eBipedMatrix D_00038538;
extern "C" void *__vec_new(void *, int, int, void *);
extern "C" void *eSavedContact__eSavedContact_void__00208928(void *);

#pragma control sched=2

eBipedController::eBipedController(cBase *parent) : ePhysicsController(parent) {
    *(void **)((char *)this + 4) = eBipedControllerclassdesc;
    bodyIndex = 0;
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
    bodyEntries = 0;
    *(unsigned char *)((char *)this + 0x34) = 0;
    *(unsigned char *)((char *)this + 0x35) = 0;
    *(int *)((char *)this + 0x38) = 0;
    *(unsigned char *)((char *)this + 0x3C) = 0;
    __asm__ volatile("swc1 $f12, 0x40(%0)" : : "r"(this) : "memory");
    *(int *)((char *)this + 0x44) = 0;
    *(int *)((char *)this + 0x48) = 0;
    __asm__ volatile(
        "swc1 $f12, 0x90(%0)\n"
        "mfc1 $4, $f12\n"
        "mfc1 $5, $f12\n"
        "mfc1 $6, $f12\n"
        "mtv $4, S120\n"
        "mtv $5, S121\n"
        "mtv $6, S122\n"
        "sv.q C120, 0xA0(%0)\n"
        :
        : "r"(this)
        : "$4", "$5", "$6", "memory");
    volatile eBipedMatrix local;
    volatile eBipedMatrix init;
    if (D_00038568 == 0) {
        D_00038568 = 1;
        __asm__ volatile(
            "lui $6, 0x3f80\n"
            "mtc1 $6, $f13\n"
            "mfc1 $6, $f13\n"
            "mfc1 $7, $f12\n"
            "mfc1 $8, $f12\n"
            "mtv $6, S120\n"
            "mtv $7, S121\n"
            "mtv $8, S122\n"
            "sv.q C120, 0x30($29)\n"
            "mfc1 $6, $f12\n"
            "mfc1 $7, $f13\n"
            "mfc1 $8, $f12\n"
            "mtv $6, S130\n"
            "mtv $7, S131\n"
            "mtv $8, S132\n"
            "sv.q C130, 0x40($29)\n"
            "mfc1 $6, $f12\n"
            "mfc1 $7, $f12\n"
            "mfc1 $8, $f13\n"
            "mtv $6, S200\n"
            "mtv $7, S201\n"
            "mtv $8, S202\n"
            "sv.q C200, 0x50($29)\n"
            "sv.q C120, 0x0(%1)\n"
            "sv.q C130, 0x10(%1)\n"
            "sv.q C200, 0x20(%1)\n"
            : "=m"(init)
            : "r"(&D_00038538)
            : "$6", "$7", "$8", "$f13", "memory");
    }
    __asm__ volatile(
        "lv.q C120, 0x0(%1)\n"
        "sv.q C120, 0x0($29)\n"
        "lv.q C130, 0x10(%1)\n"
        "sv.q C130, 0x10($29)\n"
        "lv.q C130, 0x20(%1)\n"
        "sv.q C130, 0x20($29)\n"
        "sv.q C120, 0xB0(%2)\n"
        "lv.q C120, 0x10($29)\n"
        "sv.q C120, 0xC0(%2)\n"
        "lv.q C120, 0x20($29)\n"
        "sv.q C120, 0xD0(%2)\n"
        : "=m"(local)
        : "r"(&D_00038538), "r"(this)
        : "$6", "$7", "memory");
    *(unsigned char *)((char *)this + 0xE0) = 0;
    __asm__ volatile(
        "mfc1 $4, $f12\n"
        "mfc1 $5, $f12\n"
        "mfc1 $6, $f12\n"
        "mtv $4, S120\n"
        "mtv $5, S121\n"
        "mtv $6, S122\n"
        "sv.q C120, 0xF0(%0)\n"
        "swc1 $f12, 0x110(%0)\n"
        :
        : "r"(this)
        : "$4", "$5", "$6", "memory");
    *(int *)((char *)this + 0x120) = 0;
    __vec_new((char *)this + 0x130, 8, 0x30,
              (void *)eSavedContact__eSavedContact_void__00208928);
    *(int *)((char *)this + 0x2B0) = 0;
    *(int *)((char *)this + 0x2B4) = 0;
}

#pragma control sched=1

eBipedController::~eBipedController() {
    *(void **)((char *)this + 4) = eBipedControllerclassdesc;
    void *field = (char *)this + 0x30;
    if (field != 0) {
        void *entries = bodyEntries;
        int count = 0;
        if (entries != 0) {
            count = *(int *)((char *)entries - 4) & 0x3FFFFFFF;
        }
        int i = 0;
        if (i < count) {
            do {
                i++;
            } while (i < count);
        }
        if (entries != 0) {
            char *basePtr = (char *)entries - 4;
            if (basePtr != 0) {
                cMemPool *pool = cMemPool::GetPoolFromPtr(basePtr);
                char *block = ((char **)pool)[9];
                eBipedControllerDeleteRec *rec =
                    (eBipedControllerDeleteRec *)(((char **)block)[7] + 0x30);
                short off = rec->offset;
                __asm__ volatile("" ::: "memory");
                char *base = block + off;
                void (*fn)(void *, void *) = rec->fn;
                fn(base, basePtr);
            }
            bodyEntries = 0;
        }
    }
}

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
        new (obj) eBipedController(parent);
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
