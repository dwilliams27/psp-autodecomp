inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cFile;
class cMemPool;
class cObject;
class cReadBlock;
class cType;

class cWriteBlock {
public:
    cFile *_file;
    int _pos;
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cHandle {
public:
    int mId;
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &, cMemPool *);
};

class gcDesiredObject {
public:
    cBase *mParent;
    void *mVTable;
    unsigned int mNext;

    gcDesiredObject(cBase *);
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
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

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class gcDesiredObjectHelper {
public:
    enum gcPrimary {};
    static const char *GetPrimaryText(gcPrimary);
};

extern "C" void cStrCat(char *, const char *);
extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

class gcEnumeration {
public:
    static cBase *New(cMemPool *, cBase *);
};

extern char gcDesiredEnumerationvirtualtable[];

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_0009F3F4;
extern cType *D_0009F450;
extern cType *D_000998F0;

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcDesiredEnumeration : public gcDesiredObject {
public:
    int mType;
    cHandle mHandle;

    cObject *Get(bool) const;
    cObject *GetObject(bool) const;
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    void GetText(char *) const;
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    const cType *GetDesiredType(void) const;
};

cObject *gcDesiredEnumeration::GetObject(bool b) const {
    return Get(b);
}

int gcDesiredEnumeration::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    int inner[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 1, true);
    if (rb[3] != 1 || ((gcDesiredObject *)this)->Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    __0oKcReadBlockctR6FcFileUib(inner, *(cFile *)rb[0], 3, true);
    cFileSystem::Read(*(void **)inner[0], (char *)this + 0x0C, 4);
    *(int *)((char *)this + 0x10) = 0;
    {
        cHandle *handle = (cHandle *)((char *)this + 0x10);
        handle->Read(*(cReadBlock *)inner, cMemPool::GetPoolFromPtr(handle));
    }
    __0oKcReadBlockdtv(inner, 2);
    __0oKcReadBlockdtv(rb, 2);
    return result;
}

cBase *gcDesiredEnumeration::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDesiredEnumeration *result = 0;
    gcDesiredEnumeration *obj = (gcDesiredEnumeration *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        new ((gcDesiredObject *)obj) gcDesiredObject(parent);
        obj->mVTable = gcDesiredEnumerationvirtualtable;
        obj->mType = 7;
        obj->mHandle.mId = 0;
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDesiredEnumeration::GetType(void) const {
    if (D_0009F450 == 0) {
        if (D_0009F3F4 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894, (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F3F4 = cType::InitializeType(0, 0, 0x12C, D_000385DC, 0, 0, 0, 0);
        }
        D_0009F450 = cType::InitializeType(0, 0, 0x2C1, D_0009F3F4,
                                           &gcDesiredEnumeration::New, 0, 0, 0);
    }
    return D_0009F450;
}

// ── gcDesiredEnumeration::GetText(char *) const @ 0x001286f4 ──
void gcDesiredEnumeration::GetText(char *buf) const {
    int v8 = *(int *)((const char *)this + 8);
    int flag1 = 0;
    if (v8 & 1) {
        flag1 = 1;
    }
    int hasReal;
    if (flag1 != 0) {
        hasReal = 0;
    } else {
        int raw = (v8 != 0);
        hasReal = ((unsigned char)raw) != 0;
    }
    if (hasReal == 0) {
        int t = *(int *)((const char *)this + 12);
        const char *text;
        if (t == 7) {
            int hid = *(int *)((const char *)this + 16);
            void *entry;
            if (hid == 0) {
                entry = 0;
            } else {
                void **table = (void **)0x38890;
                void *cand = table[(unsigned short)hid];
                entry = 0;
                if (cand != 0 && *(int *)((char *)cand + 0x30) == hid) {
                    entry = cand;
                }
            }
            if (entry != 0) {
                short v = *(short *)((char *)entry + 0x1C);
                int isZero = (v == 0);
                isZero &= 0xFF;
                if (isZero == 0) {
                    text = (const char *)((char *)entry + 8);
                } else {
                    text = (const char *)0x36DAB8;
                }
            } else if (hid != 0) {
                text = (const char *)0x36DAC4;
            } else {
                text = (const char *)0x36DACC;
            }
        } else {
            text = gcDesiredObjectHelper::GetPrimaryText((gcDesiredObjectHelper::gcPrimary)t);
        }
        cStrCat(buf, text);
    }
}

// ── gcDesiredEnumeration::GetDesiredType(void) const @ 0x00128b44 ──
const cType *gcDesiredEnumeration::GetDesiredType(void) const {
    if (D_000998F0 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType(
                        (const char *)0x36D894, (const char *)0x36D89C,
                        1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(
                    0, 0, 2, D_000385DC, &cNamed::New, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(
                0, 0, 3, D_000385E0, 0, 0, 0, 0);
        }
        D_000998F0 = cType::InitializeType(
            0, 0, 0xAB, D_000385E4, &gcEnumeration::New,
            (const char *)0x36D8A4, (const char *)0x36D8B4, 5);
    }
    return D_000998F0;
}

void gcDesiredEnumeration::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcDesiredObject *)this)->Write(file);
    cWriteBlock inner(*wb._file, 3);
    inner.Write(mType);
    mHandle.Write(inner);
    inner.End();
    wb.End();
}
