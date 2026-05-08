// eLensFlareSprite — eAll_psp.obj
//   0x0003ba38  Write(cFile &) const                       (124B)
//   0x0003bc0c  ~eLensFlareSprite(void)                    (100B)
//   0x001e8378  AssignCopy(const cBase *)                   (96B)
//   0x001e83d8  New(cMemPool *, cBase *) static            (124B)
//
// 32-byte object derived from cBase. Payload starts at offset 8: a 4-byte
// int field, a 4-byte cHandle, and four FPU scalars at +0x10..+0x1C.

class cBase;
class cFile;
class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};
class cMemPoolNS;
class cReadBlock;
class cType;

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
    void Write(unsigned int);
    void Write(float);
    void End(void);
};

class cHandle {
public:
    int mId;
    void Read(cReadBlock &, cMemPool *);
    void Write(cWriteBlock &) const;
};

inline void *operator new(unsigned int, void *p) { return p; }

// Pool-block layout (mirrors other class destructors / ::New).
struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRec {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cMemPoolNS {
public:
    static cMemPoolNS *GetPoolFromPtr(const void *);
};

class eLensFlareSprite {
public:
    cBase *mOwner;
    void *mClassDesc;
    int mField8;
    cHandle mFieldC;
    float mField10;
    float mField14;
    float mField18;
    float mField1C;

    eLensFlareSprite(cBase *);
    void AssignCopy(const cBase *);
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

template <class T> T *dcast(const cBase *);

extern cType *D_000385DC;
extern cType *D_000468C8;
void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

// ── 0x0003bbc8 — eLensFlareSprite(cBase *), 68B ──
eLensFlareSprite::eLensFlareSprite(cBase *parent) {
    mOwner = parent;
    mClassDesc = (void *)0x3807E8;
    mField8 = 0;
    mFieldC.mId = 0;
    mField10 = 0.0f;
    mField14 = 1.0f;
    mField18 = 1.0f;
    mField1C = 180.0f;
}

// ── 0x001e8378 — AssignCopy(const cBase *), 96B ──
void eLensFlareSprite::AssignCopy(const cBase *base) {
    eLensFlareSprite *o = dcast<eLensFlareSprite>(base);
    mField8 = o->mField8;
    mFieldC = o->mFieldC;
    mField10 = o->mField10;
    mField14 = o->mField14;
    mField18 = o->mField18;
    mField1C = o->mField1C;
}

// ── 0x0003bc0c — ~eLensFlareSprite(void), 100B ──
//
// Deleting-destructor variant (combined ABI form): null-check this, reset
// classdesc to parent (cBase) at 0x37E6A8, and on (flags & 1) dispatch the
// pool's delete-record at allocTable+0x30. eConfigBase-style direct dispatch
// produces the EARLY scheduling pattern this address requires.
extern "C" {

void eLensFlareSprite___dtor_eLensFlareSprite_void(void *self, int flags) {
    if (self != 0) {
        *(void **)((char *)self + 4) = (void *)0x37E6A8;
        if (flags & 1) {
            void *pool = cMemPoolNS::GetPoolFromPtr(self);
            void *block = *(void **)((char *)pool + 0x24);
            char *allocTable = *(char **)((char *)block + 0x1C);
            DeleteRec *rec = (DeleteRec *)(allocTable + 0x30);
            short off = rec->offset;
            rec->fn((char *)block + off, self);
        }
    }
}

}

// ── 0x0003ba38 — Write(cFile &) const, 124B ──
//
// Serializes payload via a type-2 cWriteBlock: int field, cHandle, and four
// floats; closes with End().
void eLensFlareSprite::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    wb.Write((unsigned int)mField8);
    mFieldC.Write(wb);
    wb.Write(mField10);
    wb.Write(mField14);
    wb.Write(mField18);
    wb.Write(mField1C);
    wb.End();
}

// ── 0x0003bab4 — Read(cFile &, cMemPool *), 276B ──
int eLensFlareSprite::Read(cFile &file, cMemPool *) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 2, true);
    if (rb[3] != 2) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }
    cFileSystem::Read(*(void **)rb[0], (char *)this + 8, 4);
    {
        mFieldC.mId = 0;
        cHandle *handle = (cHandle *)((char *)this + 0x0C);
        handle->Read(*(cReadBlock *)rb, cMemPool::GetPoolFromPtr(handle));
    }
    cFileSystem::Read(*(void **)rb[0], (char *)this + 0x10, 4);
    cFileSystem::Read(*(void **)rb[0], (char *)this + 0x14, 4);
    cFileSystem::Read(*(void **)rb[0], (char *)this + 0x18, 4);
    cFileSystem::Read(*(void **)rb[0], (char *)this + 0x1C, 4);
    __0oKcReadBlockdtv(rb, 2);
    return result;
}

// ── 0x001e83d8 — New(cMemPool *, cBase *) static, 124B ──
//
// Pool-driven allocation through allocTable+0x28: size 0x20, alignment 4.
// On success, runs the constructor and returns the new object cast to cBase*.
cBase *eLensFlareSprite::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eLensFlareSprite *result = 0;
    eLensFlareSprite *obj = (eLensFlareSprite *)rec->fn(base, 0x20, 4, 0, 0);
    if (obj != 0) {
        new (obj) eLensFlareSprite(parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── 0x001e8454 — GetType(void) const, 160B ──
const cType *eLensFlareSprite::GetType(void) const {
    if (D_000468C8 == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                               (const char *)0x36CD7C,
                                               1, 0, 0, 0, 0, 0);
        }
        D_000468C8 = cType::InitializeType(0, 0, 0x19A, D_000385DC,
                                           &eLensFlareSprite::New, 0, 0, 0);
    }
    return D_000468C8;
}
