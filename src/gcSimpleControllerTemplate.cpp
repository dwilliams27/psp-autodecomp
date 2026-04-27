class cBase;
class cFile;
class cMemPool;
class gcEntityControllerTemplate;
class gcSimpleControllerTemplate;

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
};

extern "C" {
    void gcSimpleControllerTemplate__gcSimpleControllerTemplate_cBaseptr(void *self, cBase *parent);
}

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

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
