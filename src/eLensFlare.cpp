// eLensFlare.cpp — decompiled from eAll_psp.obj.
// Parent: cObject. Vtable at 0x380858 (eLensFlarevirtualtable).
//
// Functions:
//   0x0003bdac eLensFlare::Write(cFile &) const             100B
//   0x0003beec eLensFlare::eLensFlare(cBase *)               68B
//   0x001e880c eLensFlare::AssignCopy(const cBase *)          92B
//   0x001e8868 eLensFlare::New(cMemPool *, cBase *) static  124B

class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned int);
    void End(void);
};

class cBaseArray {
public:
    cBaseArray &operator=(const cBaseArray &);
    void Write(cWriteBlock &) const;
};

class cObject {
public:
    char _pad[0x44];
    cObject(cBase *);
    cObject &operator=(const cObject &);
    void Write(cFile &) const;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

template <class T> T *dcast(const cBase *);

class eLensFlare : public cObject {
public:
    int mField44;            // 0x44 (cBaseArray.count)
    void *mField48;          // 0x48 (cBaseArray.owner — self-pointer)
    unsigned int mField4C;   // 0x4C

    eLensFlare(cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

extern char eLensFlarevirtualtable[];
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_000468CC;

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

extern "C" {
    void eLensFlare__eLensFlare_cBaseptr(void *self, cBase *parent);
}

// ── eLensFlare::AssignCopy @ 0x001e880c ──
void eLensFlare::AssignCopy(const cBase *base) {
    eLensFlare *other = dcast<eLensFlare>(base);
    ((cObject *)this)->operator=(*(const cObject *)other);
    ((cBaseArray *)((char *)this + 0x44))->operator=(
        *(const cBaseArray *)((char *)other + 0x44));
    unsigned int *srcField = (unsigned int *)((char *)other + 0x4C);
    unsigned int value = *srcField;
    unsigned int *dstField = (unsigned int *)((char *)this + 0x4C);
    *dstField = value;
}

// ── eLensFlare::Write @ 0x0003bdac ──
void eLensFlare::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    cObject::Write(file);
    ((const cBaseArray *)((char *)this + 0x44))->Write(wb);
    wb.Write(mField4C);
    wb.End();
}

// ── eLensFlare::eLensFlare @ 0x0003beec ──
eLensFlare::eLensFlare(cBase *parent) : cObject(parent) {
    *(void **)((char *)this + 4) = eLensFlarevirtualtable;
    mField44 = 0;
    mField48 = (void *)this;
    mField4C = (unsigned int)-1;
}

// ── eLensFlare::New @ 0x001e8868 ──
cBase *eLensFlare::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eLensFlare *result = 0;
    eLensFlare *obj = (eLensFlare *)rec->fn(base, 0x50, 4, 0, 0);
    if (obj != 0) {
        eLensFlare__eLensFlare_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

const cType *eLensFlare::GetType(void) const {
    if (D_000468CC == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                       (const char *)0x36CD7C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(
                    0, 0, 2, D_000385DC,
                    (cBase *(*)(cMemPool *, cBase *))&cNamed::New, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                               0, 0, 0, 0);
        }
        D_000468CC = cType::InitializeType(0, 0, 0x19B, D_000385E4,
                                           &eLensFlare::New,
                                           (const char *)0x36CDF4,
                                           (const char *)0x36CE00, 1);
    }
    return D_000468CC;
}
