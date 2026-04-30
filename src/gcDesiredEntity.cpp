// gcDesiredEntity.cpp — gcAll_psp.obj
//
// Functions in this file:
//   0x0011d67c gcDesiredEntity::Write(cFile &) const   120B
//   0x0026023c gcDesiredEntity::GetType(void) const    220B

class cBase;
class cFile;
class cMemPool;
class cType;
class gcEnumeration;
class gcEnumerationEntry;

template <class T>
class cSubHandleT;

template <class T, class U>
class cHandlePairT;

class cWriteBlock {
public:
    cFile *_file;       // +0
    int _pos;           // +4
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class gcDesiredEntityHelper {
public:
    enum gcPrimary { gcPrimary_One = 1 };
    enum gcRelationship { gcRelationship_None = 0 };

    gcDesiredEntityHelper(gcPrimary, gcRelationship, gcRelationship);
    void Write(cWriteBlock &) const;
};

class gcDesiredObject {
public:
    gcDesiredObject(cBase *);
    void Write(cFile &) const;
};

class gcEntity {
public:
    int HasCategory(const cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > &) const;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

extern cType *D_000385DC;     // shared cBase root type cache
extern cType *D_0009F3F4;     // shared gcDesiredObject type cache
extern cType *D_0009F3F8;     // gcDesiredEntity type cache

typedef void (*gcDesiredEntityWriteFn)(cBase *, cFile *);

struct gcDesiredEntityWriteSlot {
    short mOffset;                  // +0
    short _pad;                     // +2
    gcDesiredEntityWriteFn mWrite;  // +4
};

struct gcDesiredEntityTypeInfo2 {
    char _pad[0x28];                       // +0x00
    gcDesiredEntityWriteSlot mSlot;        // +0x28 — sub-object Write slot
};

class gcDesiredEntity {
public:
    gcEntity *Get(bool) const;
    int HasCategory(const cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > &) const;
    void Write(cFile &) const;
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

struct gcDesiredEntityPoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct gcDesiredEntityAllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

extern char D_00000338[];
extern char gcDesiredEntityHelperclassdesc[];
extern char cBaseclassdesc[];
extern char gcDesiredEntityvirtualtable[];
extern "C" void gcDesiredObject_gcDesiredObject(void *, cBase *);
extern "C" void gcDesiredEntityHelper_ctor(void *, int, int, int)
    __asm__("gcDesiredEntityHelper__gcDesiredEntityHelper_gcDesiredEntityHelper__gcPrimary_gcDesiredEntityHelper__gcRelationship_gcDesiredEntityHelper__gcRelationship__0011B714");

// ── gcDesiredEntity::HasCategory @ 0x0011df9c ──
int gcDesiredEntity::HasCategory(
    const cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > &category) const {
    gcEntity *entity = Get(1);
    if (entity != 0) {
        return entity->HasCategory(category);
    }
    return 0;
}

// ── gcDesiredEntity::Write @ 0x0011d67c ──
void gcDesiredEntity::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    ((const gcDesiredObject *)this)->Write(file);
    ((const gcDesiredEntityHelper *)((char *)this + 12))->Write(wb);

    gcDesiredEntityTypeInfo2 *ti = *(gcDesiredEntityTypeInfo2 **)((char *)this + 24);
    gcDesiredEntityWriteSlot *slot = &ti->mSlot;
    cBase *embedded = (cBase *)((char *)this + 20);
    slot->mWrite((cBase *)((char *)embedded + slot->mOffset), wb._file);

    wb.End();
}

// ── gcDesiredEntity::New @ 0x0026015c ──
cBase *gcDesiredEntity::New(cMemPool *pool, cBase *parent) {
    gcDesiredEntity *result = 0;
    void *block = ((void **)pool)[9];
    char *allocTable = ((gcDesiredEntityPoolBlock *)block)->allocTable;
    gcDesiredEntityAllocEntry *entry = (gcDesiredEntityAllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDesiredEntity *obj = (gcDesiredEntity *)entry->fn(base, 0x2C, 4, 0, 0);
    if (obj != 0) {
        gcDesiredObject_gcDesiredObject(obj, parent);
        *(void **)((char *)obj + 4) = D_00000338;
        void *helper = (char *)obj + 0x0C;
        int one = 1;
        gcDesiredEntityHelper_ctor(helper, 1, 0, 0);
        *(void **)((char *)obj + 4) = gcDesiredEntityHelperclassdesc;
        *(void **)((char *)obj + 0x18) = cBaseclassdesc;
        *(void **)((char *)obj + 0x14) = obj;
        *(void **)((char *)obj + 0x18) = gcDesiredEntityvirtualtable;
        *(unsigned char *)((char *)obj + 0x1C) = one;
        *(unsigned char *)((char *)obj + 0x1D) = 0;
        *(int *)((char *)obj + 0x20) = 0;
        *(int *)((char *)obj + 0x24) = 0;
        *(int *)((char *)obj + 0x28) = (int)obj | 1;
        result = obj;
    }
    return (cBase *)result;
}

// ── gcDesiredEntity::GetType @ 0x0026023c ──
const cType *gcDesiredEntity::GetType(void) const {
    if (D_0009F3F8 == 0) {
        if (D_0009F3F4 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894, (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F3F4 = cType::InitializeType(0, 0, 0x12C, D_000385DC, 0, 0, 0, 0);
        }
        D_0009F3F8 = cType::InitializeType(0, 0, 0x12D, D_0009F3F4,
                                           (cBase *(*)(cMemPool *, cBase *))&gcDesiredEntity::New,
                                           0, 0, 0);
    }
    return D_0009F3F8;
}
