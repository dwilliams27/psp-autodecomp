class cBase;
class cMemPool;
class cType;
class eMaterial;
class cFile;

class cWriteBlock {
public:
    int _data[2];

    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(unsigned int);
    void Write(float);
    void End(void);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cObject {
public:
    cObject(cBase *);
    cObject &operator=(const cObject &);
    void Write(cFile &) const;
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

template <class T> T *dcast(const cBase *);

template <class T>
class cHandleT {
public:
    int mIndex;
};

template <class T>
class cArrayBase {
public:
    void *mData;
    cArrayBase &operator=(const cArrayBase &);
};

class cHandle {
public:
    int _data;

    void Write(cWriteBlock &) const;
};

class eFontCharKern {
public:
    char _data[0xE];

    void Write(cWriteBlock &) const;
};

inline void *operator new(unsigned int, void *p) {
    return p;
}

class eFont : public cObject {
public:
    class ePageInfo;

    eFont(cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00041114;

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

extern "C" void eFont__eFont_cBaseptr(void *, cBase *);

// -- eFont::eFont(cBase *) @ 0x00039710 --
eFont::eFont(cBase *parent) : cObject(parent) {
    *(void **)((char *)this + 4) = (void *)0x380748;
    *(int *)((char *)this + 0x44) = 0;
    ((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x48))->mData = 0;
    *(float *)((char *)this + 0x4C) = 1.0f;
    *(float *)((char *)this + 0x50) = 16.0f;
    *(float *)((char *)this + 0x54) = 16.0f;
    *(float *)((char *)this + 0x58) = 0.0f;
    ((cArrayBase<eFontCharKern> *)((char *)this + 0x5C))->mData = 0;
    ((cArrayBase<eFont::ePageInfo> *)((char *)this + 0x60))->mData = 0;
}

// -- eFont::Write(cFile &) const @ 0x00039278 --
void eFont::Write(cFile &file) const {
    int var_a0;
    int var_s0;
    int var_s1;
    int var_s1_2;
    int var_s1_3;
    int var_s2;
    void *temp_a0;
    void *temp_a0_2;
    void *temp_a0_3;
    void *temp_a1;
    void *var_s2_2;
    void *var_s3;

    cWriteBlock wb(file, 8);
    ((const cObject *)this)->Write(file);

    temp_a0 = ((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x48))->mData;
    var_s1 = 0;
    if (temp_a0 != 0) {
        var_s1 = *(int *)((char *)temp_a0 - 4) & 0x3FFFFFFF;
    }
    wb.Write(var_s1);

    temp_a0_2 = ((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x48))->mData;
    var_s1_2 = 0;
    if (temp_a0_2 != 0) {
        var_s1_2 = *(int *)((char *)temp_a0_2 - 4) & 0x3FFFFFFF;
    }
    int *baseHandles = (int *)temp_a0_2;
    var_s2 = 0;
    if (var_s2 < var_s1_2) {
        int offset = 0;
        int *handle = baseHandles + offset;
        do {
            ((const cHandle *)handle)->Write(wb);
            var_s2 += 1;
            handle++;
        } while (var_s2 < var_s1_2);
    }

    wb.Write(*(const float *)((const char *)this + 0x4C));
    wb.Write(*(const float *)((const char *)this + 0x50));
    wb.Write(*(const float *)((const char *)this + 0x54));
    wb.Write(*(const unsigned int *)((const char *)this + 0x44));
    wb.Write(*(const float *)((const char *)this + 0x58));

    temp_a1 = ((cArrayBase<eFontCharKern> *)((char *)this + 0x5C))->mData;
    var_a0 = 0;
    if (temp_a1 != 0) {
        var_a0 = *(int *)((char *)temp_a1 - 4) & 0x3FFFFFFF;
    }
    wb.Write(var_a0);

    temp_a0_3 = ((cArrayBase<eFontCharKern> *)((char *)this + 0x5C))->mData;
    var_s0 = 0;
    if (temp_a0_3 != 0) {
        var_s0 = *(int *)((char *)temp_a0_3 - 4) & 0x3FFFFFFF;
    }
    char *baseKerns = (char *)temp_a0_3;
    var_s1_3 = 0;
    if (var_s1_3 < var_s0) {
        int offset = 0;
        eFontCharKern *kern = (eFontCharKern *)(baseKerns + offset);
        do {
            kern->Write(wb);
            var_s1_3 += 1;
            kern = (eFontCharKern *)((char *)kern + 0xE);
        } while (var_s1_3 < var_s0);
    }

    wb.End();
}

// -- eFont::AssignCopy(const cBase *) @ 0x001e7948 --
void eFont::AssignCopy(const cBase *src) {
    eFont &other = *dcast<eFont>(src);
    ((cObject *)this)->operator=(*(const cObject *)&other);
    *(int *)((char *)this + 0x44) = *(int *)((char *)&other + 0x44);
    ((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x48))->operator=(*(cArrayBase<cHandleT<eMaterial> > *)((char *)&other + 0x48));
    *(float *)((char *)this + 0x4C) = *(float *)((char *)&other + 0x4C);
    *(float *)((char *)this + 0x50) = *(float *)((char *)&other + 0x50);
    *(float *)((char *)this + 0x54) = *(float *)((char *)&other + 0x54);
    *(float *)((char *)this + 0x58) = *(float *)((char *)&other + 0x58);
    ((cArrayBase<eFontCharKern> *)((char *)this + 0x5C))->operator=(*(cArrayBase<eFontCharKern> *)((char *)&other + 0x5C));
    ((cArrayBase<eFont::ePageInfo> *)((char *)this + 0x60))->operator=(*(cArrayBase<eFont::ePageInfo> *)((char *)&other + 0x60));
}

// -- eFont::New(cMemPool *, cBase *) static @ 0x001e79d4 --
cBase *eFont::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eFont *result = 0;
    eFont *obj = (eFont *)rec->fn(base, 0x64, 4, 0, 0);
    if (obj != 0) {
        new (obj) eFont(parent);
        result = obj;
    }
    return (cBase *)result;
}

#pragma control sched=1
const cType *eFont::GetType(void) const {
    if (D_00041114 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    const char *name = (const char *)0x36CD74;
                    const char *desc = (const char *)0x36CD7C;
                    __asm__ volatile("" : "+r"(name), "+r"(desc));
                    D_000385DC = cType::InitializeType(name, desc,
                                                       1, 0, 0, 0, 0, 0);
                }
                const cType *parentType = D_000385DC;
                cBase *(*factory)(cMemPool *, cBase *) =
                    (cBase *(*)(cMemPool *, cBase *))0x1C3C58;
                D_000385E0 = cType::InitializeType(0, 0, 2, parentType, factory,
                                                   0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                               0, 0, 0, 0);
        }
        const cType *parentType = D_000385E4;
        cBase *(*factory)(cMemPool *, cBase *) =
            (cBase *(*)(cMemPool *, cBase *))0x1E79D4;
        const char *kindName = (const char *)0x36CDE8;
        const char *kindDesc = (const char *)0x36CDF0;
        D_00041114 = cType::InitializeType(0, 0, 0x40, parentType, factory,
                                           kindName, kindDesc, 5);
    }
    return D_00041114;
}
#pragma control sched=2
