// eDynamicGeomTemplate.cpp — decompiled from eAll_psp.obj
// eDynamicGeomTemplate inherits from eGeomTemplate.

class cBase;
class cFile;
class cMemPool;

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
    static void Read(void *handle, void *buf, unsigned int size);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cObject {
public:
    ~cObject(void);
};

class eGeomTemplate {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class eDynamicGeomTemplate : public cObject {
public:
    ~eDynamicGeomTemplate(void);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void *target = block + off;
        __asm__ volatile("" ::: "memory");
        void (*fn)(void *, void *) = rec->fn;
        fn(target, p);
    }
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

extern char eGeomTemplatevirtualtable[];

#pragma control sched=1
// ── eDynamicGeomTemplate::Write(cFile &) const @ 0x0004B0F8 ──
void eDynamicGeomTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const eGeomTemplate *)this)->Write(file);
    wb.Write(*(float *)((char *)this + 0x44));
    wb.End();
}

// ── eDynamicGeomTemplate::Read(cFile &, cMemPool *) @ 0x0004B150 ──
int eDynamicGeomTemplate::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 2, true);
    if ((unsigned int)rb._data[3] == 2 && ((eGeomTemplate *)this)->Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    {
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" ::: "memory");
        cFileSystem::Read(h, (char *)this + 0x44, 4);
    }
    return result;
}

// ── eDynamicGeomTemplate::~eDynamicGeomTemplate(void) @ 0x001F0DB8 ──
eDynamicGeomTemplate::~eDynamicGeomTemplate(void) {
    *(void **)((char *)this + 4) = eGeomTemplatevirtualtable;
}
