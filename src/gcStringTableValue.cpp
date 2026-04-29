// Days of Thunder decompilation: gcStringTableValue methods

class cBase {
public:
    int _b0;
    int _b4;
};

class cFile;
class cMemPool;

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class cWriteBlock {
public:
    cFile *file;
    unsigned int _d1;
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

struct cTypeMethod {
    short offset;
    short pad;
    void *fn;
};

class cType {
public:
    char _p0[0x28];
    cTypeMethod write_m;   // 0x28
    char _p1[0x10];
    cTypeMethod name_m;    // 0x40
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcDesiredObject {
public:
    int _p0;
    cType *mType;
};

class gcString;
class gcStringTableValue;

class gcDesiredString : public gcDesiredObject {
public:
    char _body[0x18];
    gcDesiredString &operator=(const gcDesiredString &);
    gcString *Get(bool) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
};

class gcStringValue : public cBase {
public:
    void Write(cFile &) const;
};

class gcStringTableValue : public gcStringValue {
public:
    gcDesiredString mDesired;
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    void Get(wchar_t *, int) const;
    void GetName(char *) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
};

class gcString {
public:
    char _pad[0x18];
    const wchar_t *mText;
};

gcStringTableValue *dcast(const cBase *);
void cStrCopy(wchar_t *, const wchar_t *, int);
extern const wchar_t gcStringTableValueEmptyStr[];
void gcDesiredObject_gcDesiredObject(void *, cBase *);
extern char cBasevirtualtable[];
extern char gcStringValuevirtualtable[];
extern char gcStringTableValuevirtualtable[];
extern cType *D_000385DC;
extern cType *D_0009F454;
extern cType *D_0009F56C;

typedef void (*WriteFn)(void *, cFile *);
typedef void (*NameFn)(void *, char *);

// ── gcStringTableValue::New(cMemPool *, cBase *) static @ 0x0028995C ──
cBase *gcStringTableValue::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcStringTableValue *result = 0;
    gcStringTableValue *obj = (gcStringTableValue *)entry->fn(base, 0x24, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = cBasevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = gcStringValuevirtualtable;
        void *desired = (char *)obj + 8;
        gcDesiredObject_gcDesiredObject(desired, obj);
        ((void **)obj)[3] = gcStringTableValuevirtualtable;
        ((unsigned char *)obj)[0x14] = 0;
        ((unsigned char *)obj)[0x15] = 0;
        ((int *)obj)[6] = 0;
        ((int *)obj)[7] = 0;
        ((int *)obj)[8] = (int)((unsigned int)desired | 1);
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcStringTableValue::GetType(void) const {
    if (D_0009F56C == 0) {
        if (D_0009F454 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F454 = cType::InitializeType(0, 0, 0x170, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F56C = cType::InitializeType(0, 0, 0x17B, D_0009F454,
                                           &gcStringTableValue::New, 0, 0, 0);
    }
    return D_0009F56C;
}

void gcStringTableValue::GetName(char *buf) const {
    const cTypeMethod *e = &this->mDesired.mType->name_m;
    char *base = (char *)this + 8;
    ((NameFn)e->fn)(base + e->offset, buf);
}

void gcStringTableValue::AssignCopy(const cBase *other) {
    gcStringTableValue *src = dcast(other);
    gcDesiredString &srcDesired = *(gcDesiredString *)((char *)src + 8);
    ((gcDesiredString *)((char *)this + 8))->operator=(srcDesired);
}

void gcStringTableValue::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    this->gcStringValue::Write(file);
    const cTypeMethod *e = &this->mDesired.mType->write_m;
    char *base = (char *)this + 8;
    ((WriteFn)e->fn)(base + e->offset, wb.file);
    wb.End();
}

void gcStringTableValue::Get(wchar_t *buf, int size) const {
    gcString *p = this->mDesired.Get(true);
    const wchar_t *src;
    if (p != 0) {
        src = (p->mText != 0) ? p->mText : gcStringTableValueEmptyStr;
    } else {
        src = gcStringTableValueEmptyStr;
    }
    cStrCopy(buf, src, size);
}

void gcStringTableValue::VisitReferences(unsigned int flags, cBase *ctx, void (*cb)(cBase *, unsigned int, void *), void *user, unsigned int mask) {
    if (cb != 0) {
        cb(ctx, (unsigned int)(void *)this, user);
    }
    this->mDesired.VisitReferences(flags, (cBase *)this, cb, user, mask);
}
