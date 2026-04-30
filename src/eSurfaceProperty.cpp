// eSurfaceProperty.cpp — decompiled from eAll_psp.obj
// eSurfaceProperty inherits from cNamed.
//   0x000536a4  eSurfaceProperty::Read(cFile &, cMemPool *)             (208B)
//   0x001f5f84  eSurfaceProperty::AssignCopy(const cBase *)             (112B)

class cBase;
class cFile;
class cMemPool;
class cType;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
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

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned int);
    void End(void);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);

template <class T> T *dcast(const cBase *);

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
};

class eSurfaceProperty : public cNamed {
public:
    const cType *GetType(void) const;
    int Read(cFile &, cMemPool *);
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_00046A14;

#pragma control sched=1

// ── eSurfaceProperty::Write(cFile &) const @ 0x0005364c ──
void eSurfaceProperty::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cNamed::Write(file);
    wb.Write(*(const unsigned int *)((const char *)this + 0x20));
    wb.End();
}

// ── eSurfaceProperty::Read(cFile &, cMemPool *) @ 0x000536a4 ──
int eSurfaceProperty::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 && ((cNamed *)this)->Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    {
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" ::: "memory");
        cFileSystem::Read(h, (char *)this + 0x20, 4);
    }
    return result;
}

// ── eSurfaceProperty::AssignCopy(const cBase *) @ 0x001f5f84 ──
#pragma control sched=1
void eSurfaceProperty::AssignCopy(const cBase *base) {
    struct cName { int m[6]; };
    struct Tail { int x; };
    eSurfaceProperty *other = dcast<eSurfaceProperty>(base);
    *(cName *)((char *)this + 8) = *(const cName *)((char *)other + 8);
    __asm__ volatile("" ::: "memory");
    *(Tail *)((char *)this + 0x20) = *(const Tail *)((char *)other + 0x20);
}

// ── eSurfaceProperty::New(cMemPool *, cBase *) static @ 0x001f5ff4 ──
cBase *eSurfaceProperty::New(cMemPool *pool, cBase *parent) {
    eSurfaceProperty *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eSurfaceProperty *obj = (eSurfaceProperty *)entry->fn(base, 0x24, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = (void *)0x37E6A8;
        *(cBase * volatile *)obj = parent;
        ((void **)obj)[1] = (void *)0x37E8A8;
        *(short *)((char *)obj + 0x1C) = 0;
        *(short *)((char *)obj + 0x1E) = 0;
        *(unsigned char *)((char *)obj + 8) = 0;
        ((void **)obj)[1] = (void *)0x381C08;
        *(int *)((char *)obj + 0x20) = 0;
        result = obj;
    }
    return (cBase *)result;
}

// ── eSurfaceProperty::GetType(void) const @ 0x001f6094 ──
const cType *eSurfaceProperty::GetType(void) const {
    if (D_00046A14 == 0) {
        if (D_000385E0 == 0) {
            if (D_000385DC == 0) {
                const char *name = (const char *)0x36CD74;
                const char *desc = (const char *)0x36CD7C;
                __asm__ volatile("" : "+r"(name), "+r"(desc));
                D_000385DC = cType::InitializeType(name, desc, 1, 0, 0, 0, 0, 0);
            }
            const cType *parentType = D_000385DC;
            cBase *(*factory)(cMemPool *, cBase *) = &cNamed::New;
            __asm__ volatile("" : "+r"(parentType), "+r"(factory));
            D_000385E0 = cType::InitializeType(0, 0, 2, parentType,
                                               factory, 0, 0, 0);
        }
        const cType *parentType = D_000385E0;
        cBase *(*factory)(cMemPool *, cBase *) = &eSurfaceProperty::New;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046A14 = cType::InitializeType(0, 0, 0x2B7, parentType,
                                           factory, 0, 0, 0);
    }
    return D_00046A14;
}
