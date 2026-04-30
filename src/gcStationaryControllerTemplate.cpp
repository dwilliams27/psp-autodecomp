class cBase;
class cFile;
class cType;
class gcEntityControllerTemplate;
class gcStationaryControllerTemplate;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
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

void cFile_SetCurrentPos(void *, unsigned int);

class cBaseArray {
public:
    int _count;
    cBase *_owner;
    void RemoveAll(void);
    cBaseArray &operator=(const cBaseArray &);
};

class gcEntityControllerTemplate {
public:
    int base;

    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcStationaryControllerTemplate : public gcEntityControllerTemplate {
public:
    ~gcStationaryControllerTemplate();
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
};

extern "C" {
    void gcStationaryControllerTemplate__gcStationaryControllerTemplate_cBaseptr(void *self, cBase *parent);
    void *dcastdcast_gcStationaryControllerTemplateptr__constcBaseptr(const cBase *);
}

extern cType *D_000385DC;
extern cType *D_0009A400;
extern cType *D_0009F7C8;

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRec {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

inline void operator delete(void *p) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(p);
    void *block = *(void **)((char *)pool + 0x24);
    DeleteRec *rec = (DeleteRec *)(*(char **)((char *)block + 0x1C) + 0x30);
    short off = rec->offset;
    rec->fn((char *)block + off, p);
}

extern char gcEntityControllerTemplate_dtor_classdesc[];
extern char cBase_dtor_classdesc[];

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oegcStationaryControllerTemplatedtv, 0xb0\n");

// ── gcStationaryControllerTemplate::~gcStationaryControllerTemplate(void) @ 0x0032059c ──
gcStationaryControllerTemplate::~gcStationaryControllerTemplate() {
    *(char **)((char *)this + 4) = gcEntityControllerTemplate_dtor_classdesc;
    cBaseArray *arr1 = (cBaseArray *)((char *)this + 0x1C);
    cBaseArray *arr0 = (cBaseArray *)((char *)this + 0x08);
    if (arr1 != 0) {
        arr1->RemoveAll();
    }
    if (arr0 != 0) {
        arr0->RemoveAll();
    }
    *(char **)((char *)this + 4) = cBase_dtor_classdesc;
}

// ── gcStationaryControllerTemplate::Write(cFile &) const @ 0x001581a0 ──
void gcStationaryControllerTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    this->gcEntityControllerTemplate::Write(file);
    wb.End();
}

// ── gcStationaryControllerTemplate::Read(cFile &, cMemPool *) @ 0x001581ec ──
int gcStationaryControllerTemplate::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] != 1 || !this->gcEntityControllerTemplate::Read(file, pool)) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return 0;
    }
    return result;
}

// ── gcStationaryControllerTemplate::New(cMemPool *, cBase *) static @ 0x00320444 ──
cBase *gcStationaryControllerTemplate::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcStationaryControllerTemplate *result = 0;
    gcStationaryControllerTemplate *obj = (gcStationaryControllerTemplate *)rec->fn(base, 0x50, 0x10, 0, 0);
    if (obj != 0) {
        gcStationaryControllerTemplate__gcStationaryControllerTemplate_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── gcStationaryControllerTemplate::AssignCopy(const cBase *) @ 0x003203a8 ──
typedef int v4sf_t __attribute__((mode(V4SF)));

void gcStationaryControllerTemplate::AssignCopy(const cBase *src) {
    gcStationaryControllerTemplate *other =
        (gcStationaryControllerTemplate *)dcastdcast_gcStationaryControllerTemplateptr__constcBaseptr(src);

    const cBaseArray &srcArr0 = *(const cBaseArray *)((char *)other + 8);
    ((cBaseArray *)((char *)this + 8))->operator=(srcArr0);

    int i = 0;
    int *dst = (int *)((char *)this + 16);
    int *srcp = (int *)((char *)other + 16);
    do {
        i++;
        *dst = *srcp;
        dst++;
        srcp++;
    } while (i < 2);

    *(int *)((char *)this + 24) = *(const int *)((char *)other + 24);

    ((cBaseArray *)((char *)this + 28))->operator=(*(const cBaseArray *)((char *)other + 28));

    *(v4sf_t *)((char *)this + 0x30) = *(const v4sf_t *)((char *)other + 0x30);

    *(float *)((char *)this + 0x40) = *(const float *)((char *)other + 0x40);
    *(float *)((char *)this + 0x44) = *(const float *)((char *)other + 0x44);
    *(float *)((char *)this + 0x48) = *(const float *)((char *)other + 0x48);
}

// ── gcStationaryControllerTemplate::GetType(void) const @ 0x003204c0 ──
const cType *gcStationaryControllerTemplate::GetType(void) const {
    if (D_0009F7C8 == 0) {
        if (D_0009A400 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009A400 = cType::InitializeType(0, 0, 0x9A, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F7C8 = cType::InitializeType(0, 0, 0x102, D_0009A400,
                                           &gcStationaryControllerTemplate::New,
                                           0, 0, 0);
    }
    return D_0009F7C8;
}
