class cBase;
class cFile;
class cMemPool;
class cType;
class gcEntityControllerTemplate;
class gcSimpleControllerTemplate;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

void cFile_SetCurrentPos(void *, unsigned int);

class gcEntityControllerTemplate {
public:
    int base;

    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcSimpleControllerTemplate : public gcEntityControllerTemplate {
public:
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    const cType *GetType(void) const;
};

extern "C" {
    void gcSimpleControllerTemplate__gcSimpleControllerTemplate_cBaseptr(void *self, cBase *parent);
}

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

extern cType *D_000385DC;
extern cType *D_0009A400;
extern cType *D_0009F7C0;

// ── gcSimpleControllerTemplate::Write(cFile &) const @ 0x00157cfc ──
void gcSimpleControllerTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    this->gcEntityControllerTemplate::Write(file);
    wb.End();
}

// ── gcSimpleControllerTemplate::Read(cFile &, cMemPool *) @ 0x00157d48 ──
int gcSimpleControllerTemplate::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] != 1 || !this->gcEntityControllerTemplate::Read(file, pool)) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return 0;
    }
    return result;
}

// ── gcSimpleControllerTemplate::New(cMemPool *, cBase *) static @ 0x0031fdec ──
cBase *gcSimpleControllerTemplate::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcSimpleControllerTemplate *result = 0;
    gcSimpleControllerTemplate *obj = (gcSimpleControllerTemplate *)rec->fn(base, 0x50, 0x10, 0, 0);
    if (obj != 0) {
        gcSimpleControllerTemplate__gcSimpleControllerTemplate_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── gcSimpleControllerTemplate::GetType(void) const @ 0x0031fe68 ──
const cType *gcSimpleControllerTemplate::GetType(void) const {
    if (D_0009F7C0 == 0) {
        if (D_0009A400 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009A400 = cType::InitializeType(0, 0, 0x9A, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F7C0 = cType::InitializeType(0, 0, 0xF3, D_0009A400,
                                           &gcSimpleControllerTemplate::New,
                                           0, 0, 0);
    }
    return D_0009F7C0;
}
