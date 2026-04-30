// eGeomTemplate.cpp — decompiled from eAll_psp.obj
// Parent: cObject. Vtable at 0x380C18 (eGeomTemplatevirtualtable).

class cBase;
class cFile;
class cMemPool;
class cType;

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cObject {
public:
    cObject(cBase *);
    ~cObject(void);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
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

extern "C" void cFile_SetCurrentPos(void *, unsigned int);

class eGeomTemplate : public cObject {
public:
    eGeomTemplate(cBase *);
    ~eGeomTemplate(void);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    const cType *GetType(void) const;
};

extern char eGeomTemplatevirtualtable[];
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_000469A8;

// ── eGeomTemplate::Write(cFile &) const @ 0x00040E30 ──
void eGeomTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cObject::Write(file);
    wb.End();
}

// ── eGeomTemplate::Read(cFile &, cMemPool *) @ 0x00040E7C ──
int eGeomTemplate::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && cObject::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── eGeomTemplate::GetType(void) const @ 0x001EBD20 ──
const cType *eGeomTemplate::GetType(void) const {
    if (D_000469A8 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                       (const char *)0x36CD7C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                                   &cNamed::New, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                               0, 0, 0, 0);
        }
        D_000469A8 = cType::InitializeType(0, 0, 0x20, D_000385E4,
                                           0, (const char *)0x36CE2C,
                                           (const char *)0x36CE3C, 5);
    }
    return D_000469A8;
}

// ── eGeomTemplate::~eGeomTemplate(void) @ 0x001EBE44 ──
// Canonical C++ destructor; SNC ABI auto-generates the (this != 0) guard,
// the ~cObject() base-chain call (with flags=0), and the deleting-tail
// `if (flags & 1) operator delete(this)` epilogue.
eGeomTemplate::~eGeomTemplate(void) {
    *(void **)((char *)this + 4) = eGeomTemplatevirtualtable;
}
