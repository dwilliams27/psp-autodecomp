// gcUIButton -- decompiled from gcAll_psp.obj.
// Functions:
//   0x0013984c  ~gcUIButton(void)
//   0x0028e558  New(cMemPool *, cBase *) static
//   0x00139654  Write(cFile &) const
//   0x00139a28  IsUpdateEmpty(bool, bool) const

class cBase;
class cFile;
class cMemPool;
class cType;

inline void *operator new(unsigned int, void *p) {
    return p;
}

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class cMemPoolHelper {
public:
    static cMemPoolHelper *GetPoolFromPtr(const void *);
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
    void Write(int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock();
};

extern "C" void cFile_SetCurrentPos(void *, int);
extern "C" void cFileSystem_Read(void *, void *, unsigned int);
extern "C" void *cMemPool_GetPoolFromPtr(const void *);

class cHandle {
public:
    int mIndex;
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &, cMemPool *);
};

class gcUIWidget {
public:
    int IsUpdateEmpty(bool, bool) const;
};

class gcUIControl : public gcUIWidget {
public:
    static cBase *New(cMemPool *, cBase *);
    void *GetFocusedSpriteToDraw(void) const;
};

class gcUITextControl : public gcUIControl {
public:
    gcUITextControl(cBase *);
    ~gcUITextControl();
    static cBase *New(cMemPool *, cBase *);
    int IsUpdateEmpty(bool, bool) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcUIButton : public gcUITextControl {
public:
    gcUIButton(cBase *);
    ~gcUIButton();
    const cType *GetType(void) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    int IsUpdateEmpty(bool, bool) const;
    void *GetFocusedSpriteToDraw(void) const;

    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPoolHelper *pool = cMemPoolHelper::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

extern "C" void *__vec_new(void *, int, int, void (*)(void *));
extern char gcUIButtonvirtualtable[];
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_0009990C;
extern cType *gcUIControl__s_pType__0042CF74;
extern cType *gcUITextControl__s_pType__0042CF78;
extern cType *D_0009F40C;
extern cType *D_0009F410;
extern cType *D_0009F57C;

gcUIButton::gcUIButton(cBase *parent) : gcUITextControl(parent) {
    *(void **)((char *)this + 4) = gcUIButtonvirtualtable;
    *(int *)((char *)this + 0x110) = 0;
    __vec_new((char *)this + 0x114, 2, 4, (void (*)(void *))0x1E74A4);
}

gcUIButton::~gcUIButton() {
    *(void **)((char *)this + 4) = gcUIButtonvirtualtable;
}

const cType *gcUIButton::GetType(void) const {
    if (D_0009F57C == 0) {
        if (D_0009F410 == 0) {
            if (D_0009F40C == 0) {
                if (D_0009990C == 0) {
                    if (D_000385E0 == 0) {
                        if (D_000385DC == 0) {
                            D_000385DC = cType::InitializeType(
                                (const char *)0x36D894, (const char *)0x36D89C,
                                1, 0, 0, 0, 0, 0);
                        }
                        D_000385E0 = cType::InitializeType(
                            0, 0, 2, D_000385DC,
                            (cBase *(*)(cMemPool *, cBase *))0x1C3C58, 0, 0, 0);
                    }
                    D_0009990C = cType::InitializeType(
                        0, 0, 0x84, D_000385E0, 0, 0, 0, 0);
                }
                D_0009F40C = cType::InitializeType(
                    0, 0, 0x201, D_0009990C, gcUIControl::New, 0, 0, 0);
            }
            D_0009F410 = cType::InitializeType(
                0, 0, 0x200, D_0009F40C, gcUITextControl::New, 0, 0, 0);
        }
        D_0009F57C = cType::InitializeType(
            0, 0, 0x205, D_0009F410, gcUIButton::New, 0, 0, 0);
    }
    return D_0009F57C;
}

// -- gcUIButton::Write(cFile &) const @ 0x00139654 --
void gcUIButton::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcUITextControl *)this)->Write(file);
    wb.Write(2);
    const cHandle *handles = (const cHandle *)((const char *)this + 0x114);
    for (int i = 0; i < 2; i++) {
        handles[i].Write(wb);
    }
    wb.End();
}

// -- gcUIButton::IsUpdateEmpty(bool, bool) const @ 0x00139a28 --
int gcUIButton::IsUpdateEmpty(bool a, bool b) const {
    if (a) {
        int flag = (*(unsigned int *)((const char *)this + 0x24) & 8) != 0;
        flag &= 0xFF;
        if (flag) {
            return 0;
        }
    }
    return ((const gcUIWidget *)this)->IsUpdateEmpty(a, b);
}

// -- gcUIButton::GetFocusedSpriteToDraw(void) const @ 0x00139c2c --
void *gcUIButton::GetFocusedSpriteToDraw(void) const {
    cHandle *handles = (cHandle *)((char *)this + 0x114);
    cHandle *h = &handles[1];
    int field = h->mIndex;
    void *result;
    if (field == 0) {
        result = 0;
    } else {
        char *table_base = (char *)0x38890;
        char *entry_addr = table_base + ((field & 0xFFFF) << 2);
        void *cand = *(void **)entry_addr;
        result = 0;
        if (cand != 0 && *(int *)((char *)cand + 0x30) == field) {
            result = cand;
        }
    }
    if (result == 0) {
        result = ((const gcUIControl *)this)->GetFocusedSpriteToDraw();
    }
    return result;
}

// -- gcUIButton::Read(cFile &, cMemPool *) @ 0x001396d8 --
int gcUIButton::Read(cFile &file, cMemPool *pool) {
    cReadBlock rb(file, 1, true);
    int count;
    int result;
    __asm__("li %0,1" : "=r"(result));
    if (rb._data[3] != 1) goto fail;
    if (((gcUITextControl *)this)->Read(file, pool) == 0) goto fail;
    cFileSystem_Read(*(void **)&rb._data[0], &count, 4);
    {
        cHandle *handles = (cHandle *)((char *)this + 0x114);
        for (int i = 0; i < count; i++) {
            handles[i].mIndex = 0;
            handles[i].Read(rb, (cMemPool *)cMemPool_GetPoolFromPtr(&handles[i]));
        }
    }
    return result;
fail:
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
}

// -- gcUIButton::New(cMemPool *, cBase *) static @ 0x0028e558 --
cBase *gcUIButton::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcUIButton *result = 0;
    gcUIButton *obj = (gcUIButton *)rec->fn(base, 0x11C, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcUIButton(parent);
        result = obj;
    }
    return (cBase *)result;
}
