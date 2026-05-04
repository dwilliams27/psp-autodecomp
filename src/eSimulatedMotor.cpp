// eSimulatedMotor — derives from ePhysicsController. Functions matched here:
//   eSimulatedMotor::GetType(void) const            @ 0x002099a0  152B
//   eSimulatedMotor::Read(cFile &, cMemPool *)      @ 0x0006b61c  120B
//   eSimulatedMotor::~eSimulatedMotor(void)         @ 0x0006b6b8  128B
//
// Dtor uses the eSphereShape pattern: in-class inline operator delete
// with the __asm__ memory barrier between offset-load and base
// computation under #pragma control sched=1. This biases SNC into
// the alternate schedule that places rec_base in a1 and the
// move a1, s0 in the delay slot of jalr a2 (matches expected bytes).

class cBase;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cFile;

extern "C" void __0oKcReadBlockctR6FcFileUib(void *rb, cFile &file,
                                             unsigned int id, bool validate);
extern "C" void __0oKcReadBlockdtv(void *rb, int flags);
extern "C" void cFile_SetCurrentPos(void *file, unsigned int pos);

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

extern "C" void free(void *);

extern char ePhysicsControllerclassdesc[];

extern cType *D_000385DC;
extern cType *D_00046BCC;

class eSimulatedMotor {
public:
    const cType *GetType(void) const;
    int Read(cFile &, cMemPool *);
    ~eSimulatedMotor(void);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        if (pool != 0) {
            char *block = ((char **)pool)[9];
            DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
            short off = rec->offset;
            __asm__ volatile("" ::: "memory");
            char *base = block + off;
            void (*fn)(void *, void *) = rec->fn;
            fn(base, p);
        } else {
            free(p);
        }
    }
};

const cType *eSimulatedMotor::GetType(void) const {
    __asm__ volatile("" ::: "memory");
    if (D_00046BCC == 0) {
        if (D_000385DC == 0) {
            const char *baseName = (const char *)0x36CD74;
            __asm__ volatile("" : "+r"(baseName) :: "memory");
            const char *baseDesc = (const char *)0x36CD7C;
            __asm__ volatile("" : "+r"(baseDesc) :: "memory");
            D_000385DC = cType::InitializeType(baseName, baseDesc, 1, 0, 0, 0, 0, 0);
        }
        D_00046BCC = cType::InitializeType(0, 0, 0x261, D_000385DC, 0, 0, 0, 0);
    }
    return D_00046BCC;
}

#pragma control sched=1

int eSimulatedMotor::Read(cFile &file, cMemPool *) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 1, true);
    if (rb[3] != 1) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }
    __0oKcReadBlockdtv(rb, 2);
    return result;
}

eSimulatedMotor::~eSimulatedMotor(void) {
    *(void **)((char *)this + 4) = ePhysicsControllerclassdesc;
}

#pragma control sched=2
