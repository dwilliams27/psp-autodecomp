// eBipedControllerTemplate.cpp — decompiled from eAll_psp.obj.
// Parent: ePhysicsControllerTemplate.
//
// Functions:
//   0x00062858 eBipedControllerTemplate::Write(cFile &) const  100B
//   0x000629f4 eBipedControllerTemplate::~eBipedControllerTemplate(void)  124B
//   0x00208638 eBipedControllerTemplate::New(cMemPool *, cBase *) static  124B

class cBase;
class cFile;
class cMemPool;
class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class eBipedController {
public:
    static cBase *New(cMemPool *, cBase *);
};

inline void *operator new(unsigned int, void *p) {
    return p;
}

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(float);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cFileSystem {
public:
    static void Read(void *, void *, unsigned int);
};

void cFile_SetCurrentPos(void *, unsigned int);

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class ePhysicsControllerTemplate {
public:
    ePhysicsControllerTemplate(cBase *);
    ~ePhysicsControllerTemplate();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class eBipedControllerTemplate : public ePhysicsControllerTemplate {
public:
    char _pad[0x2C - 0x00];
    float mField2C;  // 0x2C
    float mField30;  // 0x30

    eBipedControllerTemplate(cBase *);
    ~eBipedControllerTemplate();
    const cType *GetInstanceType(void) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        void *block = *(void **)((char *)pool + 0x24);
        __asm__ volatile("" ::: "memory");
        DeleteRecord *rec =
            (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
        short off = rec->offset;
        rec->fn((char *)block + off, p);
    }
};

extern char eBipedControllerConfigvirtualtable[];

extern cType *D_000385DC;
extern cType *D_000469D8;
extern cType *D_00046BB4;

#pragma control sched=1

// ── eBipedControllerTemplate::Write @ 0x00062858 ──
void eBipedControllerTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    ePhysicsControllerTemplate::Write(file);
    wb.Write(mField30);
    wb.Write(mField2C);
    wb.End();
}

// ── eBipedControllerTemplate::Read @ 0x000628bc ──
int eBipedControllerTemplate::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 4, true);
    if ((unsigned int)rb._data[3] < 5 &&
        (unsigned int)rb._data[3] >= 3 &&
        ePhysicsControllerTemplate::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    void *h = *(void **)rb._data[0];
    __asm__ volatile("" : "+r"(h));
    cFileSystem::Read(h, &mField30, 4);
    h = *(void **)rb._data[0];
    __asm__ volatile("" : "+r"(h));
    cFileSystem::Read(h, &mField2C, 4);
    return result;
}

// ── eBipedControllerTemplate::~eBipedControllerTemplate @ 0x000629f4 ──
eBipedControllerTemplate::~eBipedControllerTemplate() {
    *(void **)((char *)this + 4) = eBipedControllerConfigvirtualtable;
}

#pragma control sched=1

// ── eBipedControllerTemplate::GetInstanceType(void) const @ 0x00062a70 ──
const cType *eBipedControllerTemplate::GetInstanceType(void) const {
    if (D_00046BB4 == 0) {
        if (D_000469D8 == 0) {
            if (D_000385DC == 0) {
                const char *name = (const char *)0x36CD74;
                const char *desc = (const char *)0x36CD7C;
                __asm__ volatile("" : "+r"(name), "+r"(desc));
                D_000385DC = cType::InitializeType(
                    name, desc, 1, 0, 0, 0, 0, 0);
            }
            D_000469D8 = cType::InitializeType(0, 0, 0x232, D_000385DC,
                                               0, 0, 0, 0);
        }
        const cType *parentType = D_000469D8;
        cBase *(*factory)(cMemPool *, cBase *) =
            (cBase *(*)(cMemPool *, cBase *))0x208DDC;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046BB4 = cType::InitializeType(0, 0, 0x238, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046BB4;
}

#pragma control sched=1

// ── eBipedControllerTemplate::New @ 0x00208638 ──
cBase *eBipedControllerTemplate::New(cMemPool *pool, cBase *parent) {
    eBipedControllerTemplate *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eBipedControllerTemplate *obj =
        (eBipedControllerTemplate *)rec->fn(base, 0x34, 4, 0, 0);
    if (obj != 0) {
        new (obj) eBipedControllerTemplate(parent);
        result = obj;
    }
    return (cBase *)result;
}
