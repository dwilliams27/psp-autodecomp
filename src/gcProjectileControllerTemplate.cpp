// gcProjectileControllerTemplate — gcAll_psp.obj
//
// Functions:
//   gcProjectileControllerTemplate::Write(cFile &) const @ 0x00155734 (172B)
//   gcProjectileControllerTemplate::gcProjectileControllerTemplate(cBase *) @ 0x00155B1C (100B)
//   gcProjectileControllerTemplate::New(cMemPool *, cBase *) static @ 0x0031EC28 (124B)

class cBase;
class cFile;
class cMemPool;
class cType;

inline void *operator new(unsigned int, void *p) { return p; }

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class cWriteBlock {
public:
    int _data[2];

    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(unsigned int);
    void Write(float);
    void End(void);
};

class gcEntityControllerTemplate {
public:
    gcEntityControllerTemplate(cBase *);
    void Write(cFile &) const;
    char _pad[0x50];
};

class gcProjectileControllerTemplate : public gcEntityControllerTemplate {
public:
    unsigned int m50;
    float m54;
    float m58;
    float m5C;
    float m60;
    float m64;
    unsigned int m68;
    bool m6C;
    char _pad6D[3];

    gcProjectileControllerTemplate(cBase *);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

extern cType *D_000385DC;
extern cType *D_0009A400;
extern cType *D_0009F7A8;
extern char gcProjectileControllerTemplatevirtualtable[];

// ── gcProjectileControllerTemplate::Write(cFile &) const @ 0x00155734 ──
void gcProjectileControllerTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 0xC);
    this->gcEntityControllerTemplate::Write(file);
    wb.Write(m68);
    wb.Write(m50);
    wb.Write(m54);
    wb.Write(m58);
    wb.Write(m5C);
    wb.Write(m60);
    wb.Write(m64);
    wb.Write(m6C);
    wb.End();
}

// ── gcProjectileControllerTemplate::gcProjectileControllerTemplate(cBase *) @ 0x00155B1C ──
gcProjectileControllerTemplate::gcProjectileControllerTemplate(cBase *parent)
    : gcEntityControllerTemplate(parent) {
    *(void **)((char *)this + 4) = gcProjectileControllerTemplatevirtualtable;
    m50 = 0;
    m54 = 0.0f;
    m58 = 1.0f;
    m5C = 0.0f;
    m60 = 0.0f;
    m64 = 0.0f;
    m68 = (unsigned int)-1;
    m6C = false;
}

// ── gcProjectileControllerTemplate::New(cMemPool *, cBase *) static @ 0x0031EC28 ──
cBase *gcProjectileControllerTemplate::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcProjectileControllerTemplate *result = 0;
    gcProjectileControllerTemplate *obj = (gcProjectileControllerTemplate *)rec->fn(base, 0x70, 0x10, 0, 0);
    if (obj != 0) {
        new (obj) gcProjectileControllerTemplate(parent);
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcProjectileControllerTemplate::GetType(void) const {
    if (D_0009F7A8 == 0) {
        if (D_0009A400 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009A400 = cType::InitializeType(0, 0, 0x9A, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F7A8 = cType::InitializeType(0, 0, 0xF1, D_0009A400,
                                           &gcProjectileControllerTemplate::New,
                                           0, 0, 0);
    }
    return D_0009F7A8;
}
