// gcDesiredEntity.cpp — gcAll_psp.obj
//
// Functions in this file:
//   0x0011d67c gcDesiredEntity::Write(cFile &) const   120B
//   0x0011d6f4 gcDesiredEntity::Read(cFile &, cMemPool *)  248B
//   0x002e0c08 gcDoLobbyFriendOp::Read(cFile &, cMemPool *)  252B
//   0x0026023c gcDesiredEntity::GetType(void) const    220B

class cBase;
class cFile;
class cFileHandle;
class cMemPool;
class cReadBlock;
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
    void Read(cReadBlock &);
    void Write(cWriteBlock &) const;
    void GetText(char *) const;
};

class gcDesiredEnumerationEntry {
public:
    int GetTextEx(char *, bool, bool) const;
};

void cStrAppend(char *, const char *, ...);

class gcDesiredObject {
public:
    gcDesiredObject(cBase *);
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
};

class gcDesiredValue {
public:
    void Read(cReadBlock &);
};

class gcAction {
public:
    int Read(cFile &, cMemPool *);
};

class gcDoLobbyFriendOp {
public:
    int Read(cFile &, cMemPool *);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
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
typedef void (*gcDesiredEntityReadFn)(cBase *, cFileHandle *, cMemPool *);

struct gcDesiredEntityWriteSlot {
    short mOffset;                  // +0
    short _pad;                     // +2
    gcDesiredEntityWriteFn mWrite;  // +4
};

struct gcDesiredEntityReadSlot {
    short mOffset;                 // +0
    short _pad;                    // +2
    gcDesiredEntityReadFn mRead;   // +4
};

struct gcDesiredEntityTypeInfo2 {
    char _pad[0x28];                       // +0x00
    gcDesiredEntityWriteSlot mSlot;        // +0x28 — sub-object Write slot
};

struct gcDesiredEntityTypeInfoRead {
    char _pad[0x30];                       // +0x00
    gcDesiredEntityReadSlot mSlot;         // +0x30 — sub-object Read slot
};

class gcDesiredEntity {
public:
    gcEntity *Get(bool) const;
    int HasCategory(const cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > &) const;
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    void GetText(char *) const;
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
extern "C" void *cMemPool_GetPoolFromPtr(const void *);
extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);
extern "C" void gcDesiredObject_gcDesiredObject(void *, cBase *);
extern "C" void gcDesiredEntityHelper_ctor(void *, int, int, int)
    __asm__("gcDesiredEntityHelper__gcDesiredEntityHelper_gcDesiredEntityHelper__gcPrimary_gcDesiredEntityHelper__gcRelationship_gcDesiredEntityHelper__gcRelationship__0011B714");

// ── gcDesiredEntity::GetText @ 0x0011d7ec ──
void gcDesiredEntity::GetText(char *buf) const {
    int val = *(const int *)((const char *)this + 8);
    int isOwned = 0;
    int valBit0 = val & 1;
    if (valBit0) {
        isOwned = 1;
    }
    if (isOwned != 0) {
        isOwned = 0;
    } else {
        isOwned = (val != 0);
        isOwned &= 0xFF;
        isOwned = isOwned != 0;
    }
    if (isOwned == 0) {
        ((const gcDesiredEntityHelper *)((const char *)this + 12))->GetText(buf);
        val = *(const int *)((const char *)this + 8);
        valBit0 = val & 1;
    }
    int flag = 0;
    if (valBit0 != 0) {
        flag = 1;
    }
    int doIt;
    if (flag != 0) {
        doIt = 1;
    } else {
        doIt = (val == 0);
        doIt &= 0xFF;
        doIt = doIt != 0;
    }
    if (doIt != 0) {
        char tmp[256];
        tmp[0] = 0;
        if (((const gcDesiredEnumerationEntry *)((const char *)this + 0x14))
                ->GetTextEx(tmp, false, false) != 0) {
            cStrAppend(buf, (const char *)0x36DE40, tmp);
        }
    }
}

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

// ── gcDesiredEntity::Read @ 0x0011d6f4 ──
int gcDesiredEntity::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 3, true);
    if (rb[3] != 3 || ((gcDesiredObject *)this)->gcDesiredObject::Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }
    ((gcDesiredEntityHelper *)((char *)this + 0x0C))->Read(*(cReadBlock *)rb);

    gcDesiredEntityTypeInfoRead *ti = *(gcDesiredEntityTypeInfoRead **)((char *)this + 0x18);
    gcDesiredEntityReadSlot *slot = &ti->mSlot;
    cBase *embedded = (cBase *)((char *)this + 0x14);
    slot->mRead((cBase *)((char *)embedded + slot->mOffset),
                (cFileHandle *)rb[0],
                (cMemPool *)cMemPool_GetPoolFromPtr(embedded));

    __0oKcReadBlockdtv(rb, 2);
    return result;
}

// ── gcDoLobbyFriendOp::Read @ 0x002e0c08 ──
int gcDoLobbyFriendOp::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    char value;
    __0oKcReadBlockctR6FcFileUib(rb, file, 2, true);
    if (rb[3] != 2 || ((gcAction *)this)->gcAction::Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x0C, 4);
    ((gcDesiredValue *)((char *)this + 0x10))->Read(*(cReadBlock *)rb);
    cFileSystem::Read(*(cFileHandle **)rb[0], &value, 1);
    *(char *)((char *)this + 0x14) = value != 0;
    __0oKcReadBlockdtv(rb, 2);
    return result;
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
