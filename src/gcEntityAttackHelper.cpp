// gcEntityAttackHelper — gcAll_psp.obj.
// Decompiled functions:
//   0x0010edb8  gcEntityAttackHelper::Write(cFile &) const                 (88B)
//   0x0025b92c  gcEntityAttackHelper::GetType(void) const                  (160B)
//   0x0025b888  gcEntityAttackHelper::New(cMemPool *, cBase *) static      (164B)

inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cFile;
class cMemPool;
class cType;

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
    void Write(bool);
    void Write(float);
    void End(void);
};

class cName {
public:
    void Write(cWriteBlock &) const;
};

class gcEntityAttackHelper {
public:
    cBase *m_parent;          // 0x00
    void  *m_vtable;          // 0x04
    char   _name_pad[0x18];   // 0x08 .. 0x1F (cName)
    float  mField20;          // 0x20
    bool   mField24;          // 0x24

    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

extern char gcEntityAttackHelper_cBase_vtable[];   // 0x37E6A8
extern char gcEntityAttackHelpervirtualtable[];    // 0x3885D8

extern cType *D_000385DC;
extern cType *D_00099A3F4;

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

// ============================================================
// 0x0010edb8 — Write(cFile &) const
// ============================================================
void gcEntityAttackHelper::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((cName *)((char *)this + 8))->Write(wb);
    wb.Write(*(const float *)((const char *)this + 0x20));
    wb.Write(*(const bool *)((const char *)this + 0x24));
    wb.End();
}

// ============================================================
// 0x0025b888 — New(cMemPool *, cBase *) static
// ============================================================
cBase *gcEntityAttackHelper::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcEntityAttackHelper *result = 0;
    gcEntityAttackHelper *obj =
        (gcEntityAttackHelper *)entry->fn(base, 0x28, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = gcEntityAttackHelper_cBase_vtable;
        ((void **)obj)[0] = parent;
        ((void **)obj)[1] = gcEntityAttackHelpervirtualtable;
        *(short *)((char *)obj + 0x1C) = 0;
        *(short *)((char *)obj + 0x1E) = 0;
        *(char *)((char *)obj + 0x08) = 0;
        *(float *)((char *)obj + 0x20) = 0.1f;
        *(char *)((char *)obj + 0x24) = 0;
        result = obj;
    }
    return (cBase *)result;
}

// ============================================================
// 0x0025b92c — GetType(void) const
// ============================================================
const cType *gcEntityAttackHelper::GetType(void) const {
    if (D_00099A3F4 == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType((const char *)0x36D894,
                                               (const char *)0x36D89C,
                                               1, 0, 0, 0, 0, 0);
        }
        D_00099A3F4 = cType::InitializeType(0, 0, 0x145, D_000385DC,
                                            &gcEntityAttackHelper::New,
                                            0, 0, 0);
    }
    return D_00099A3F4;
}
