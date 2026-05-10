// gcPlayerStrings.cpp — decompiled from gcAll_psp.obj.
// Parent: gcStringLValue.
//
// Functions:
//   0x0031e100 gcPlayerStrings::New(cMemPool *, cBase *) static  144B
//   0x0031e2a8 gcPlayerStrings::Write(cFile &) const             100B

inline void *operator new(unsigned int, void *p) { return p; }

class cFile;
class cMemPool;
class cBase;
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
    void Write(unsigned int);
    void End(void);
};

extern char gcStringLValueclassdesc[];
extern char gcPlayerStringsclassdesc[];
extern cType *D_000385DC;
extern cType *D_0009F454;
extern cType *D_0009F458;
extern cType *D_0009F7A4;
extern void *D_00038890[];

void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);
void cStrCopy(char *, const char *);

extern char *D_0037D87C;

struct FloatDispatchEntry {
    short offset;
    short pad;
    float (*fn)(void *);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct GetNameSlot {
    short offset;
    short pad;
    void (*fn)(void *, char *);
};

struct HandleEntry {
    char pad0[0x1C];
    short nameIndex;
    char pad1[0x12];
    int mId;
};

class gcStringLValue {
public:
    cBase *mOwner;       // 0
    void *mClassDesc;    // 4
    gcStringLValue(cBase *owner) {
        mClassDesc = gcStringLValueclassdesc;
        mOwner = owner;
    }
    void Write(cFile &) const;
};

class gcDesiredValue {
public:
    int mOwner;          // 0 (within gcDesiredValue)
    void Write(cWriteBlock &) const;
};

class gcPlayerStrings : public gcStringLValue {
public:
    gcDesiredValue mDesired;   // offset 8
    unsigned int mValue;       // offset 12

    gcPlayerStrings(cBase *parent) : gcStringLValue(parent) {
        mClassDesc = gcPlayerStringsclassdesc;
        *(int *)((char *)this + 8) = (int)this | 1;
        *(int *)((char *)this + 12) = 0;
    }

    void Write(cFile &) const;
    void Get(wchar_t *, int) const;
    void Set(const wchar_t *) const;
    void GetName(char *) const;
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

class gcNamedSet {
public:
    cBase *mOwner;
    void *mClassDesc;
    int mName;

    void GetName(char *) const;
};

// ── Write @ 0x0031e2a8 ──
void gcPlayerStrings::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcStringLValue::Write(file);
    mDesired.Write(wb);
    wb.Write(mValue);
    wb.End();
}

// ── gcPlayerStrings::Get(wchar_t *, int) const  @ 0x0031e3e8, 248B ──
void gcPlayerStrings::Get(wchar_t *buf, int size) const {
    extern void cStrCopy(wchar_t *, const char *, int);

    int flag = 0;
    int val = *(int *)((const char *)this + 0x08);
    if (val & 1) {
        flag = 1;
    }
    if (flag != 0) {
        val = 0;
    } else {
        __asm__ volatile("" ::: "memory");
    }

    int desiredPtr = val;
    float f;
    if (desiredPtr != 0) {
        FloatDispatchEntry *fe = (FloatDispatchEntry *)(*(char **)(desiredPtr + 4) + 0x70);
        f = fe->fn((char *)desiredPtr + fe->offset);
    } else {
        f = 0.0f;
    }
    int idx = (int)f - 1;

    char *entry = 0;
    if (idx >= 0 && idx < 8) {
        entry = D_0037D87C + idx * 0x44;
    }
    if (entry == 0) {
        entry = 0;
    } else {
        __asm__ volatile("" ::: "memory");
    }

    if (entry != 0 && this->mValue == 0) {
        cStrCopy(buf, entry + 12, size);
    }
}

// ── gcPlayerStrings::Set(const wchar_t *) const @ 0x0031e4e0, 256B ──
void gcPlayerStrings::Set(const wchar_t *src) const {
    extern void cStrCopy(char *, const wchar_t *, int);
    extern void cStrCopy(char *, const char *, int);

    char buf[0x14];

    int flag = 0;
    int val = *(int *)((const char *)this + 0x08);
    if (val & 1) {
        flag = 1;
    }
    if (flag != 0) {
        val = 0;
    } else {
        __asm__ volatile("" ::: "memory");
    }

    int desiredPtr = val;
    float f;
    if (desiredPtr != 0) {
        FloatDispatchEntry *fe = (FloatDispatchEntry *)(*(char **)(desiredPtr + 4) + 0x70);
        f = fe->fn((char *)desiredPtr + fe->offset);
    } else {
        f = 0.0f;
    }
    int idx = (int)f - 1;

    char *entry = 0;
    if (idx >= 0 && idx < 8) {
        entry = D_0037D87C + idx * 0x44;
    }
    if (entry == 0) {
        entry = 0;
    } else {
        __asm__ volatile("" ::: "memory");
    }

    if (entry != 0) {
        cStrCopy(buf, src, 0x14);
        if (this->mValue == 0) {
            cStrCopy(entry + 12, buf, 0x14);
        }
    }
}

// ── New @ 0x0031e100 ──
cBase *gcPlayerStrings::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcPlayerStrings *result = 0;
    gcPlayerStrings *obj = (gcPlayerStrings *)entry->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcPlayerStrings(parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── GetName @ 0x0031e5e0 ──
void gcPlayerStrings::GetName(char *buf) const {
    cStrAppend(buf, (const char *)0x36EBC4);

    int val = *(int *)((const char *)this + 0x08);
    int flag = 0;
    if (val & 1) {
        flag = 1;
    }
    if (flag != 0) {
        val = 0;
    } else {
        __asm__ volatile("" ::: "memory");
    }

    int check = val;
    if (check != 0) {
        char *typeInfo = *(char **)(check + 4);
        GetNameSlot *slot = (GetNameSlot *)(typeInfo + 0xD0);
        slot->fn((char *)val + slot->offset, buf);
    } else {
        cStrCat(buf, (const char *)0x36DB24);
    }

    cStrAppend(buf, (const char *)0x36E060, (const char *)0x36DAF0);
}

// ── gcNamedSet::GetName @ 0x00239758 ──
void gcNamedSet::GetName(char *buf) const {
    int handle = *(int *)((const char *)this + 0x08);
    HandleEntry *entry;
    if (handle == 0) {
        entry = 0;
    } else {
        HandleEntry *candidate = (HandleEntry *)D_00038890[handle & 0xFFFF];
        entry = 0;
        if (candidate != 0) {
            if (candidate->mId == handle) {
                entry = candidate;
            }
        }
    }

    const char *name;
    if (entry != 0) {
        unsigned int unnamed = (entry->nameIndex == 0);
        unnamed &= 0xFF;
        if (unnamed) {
            name = (const char *)0x36DAB8;
            goto object_done;
        }
        name = (const char *)entry + 0x08;
object_done:
        __asm__ volatile("" : : "r"(name));
        goto copy;
    } else if (handle != 0) {
        name = (const char *)0x36DAC4;
    } else {
        name = (const char *)0x36DACC;
    }

copy:
    ((void (*)(char *, const char *, HandleEntry *))cStrCopy)(buf, name, entry);
}

// ── GetType @ 0x0031e190 ──
const cType *gcPlayerStrings::GetType(void) const {
    if (D_0009F7A4 == 0) {
        if (D_0009F458 == 0) {
            if (D_0009F454 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_0009F454 = cType::InitializeType(0, 0, 0x170, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_0009F458 = cType::InitializeType(0, 0, 0x171, D_0009F454,
                                               0, 0, 0, 0);
        }
        D_0009F7A4 = cType::InitializeType(0, 0, 0x1E1, D_0009F458,
                                           &gcPlayerStrings::New, 0, 0, 0);
    }
    return D_0009F7A4;
}
