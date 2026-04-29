class cBase;
class cFile;
class cMemPool;
class cType;
class eGeom;
class eWeatherEffect;

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

class eGeom {
public:
    int base;

    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class eWeatherEffect : public eGeom {
public:
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    const cType *GetType(void) const;
};

extern "C" {
    void eWeatherEffect__eWeatherEffect_cBaseptr(void *self, cBase *parent);
}

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

static cType *type_cBase;
static cType *type_eGeom;
static cType *type_eWeatherEffect;

#pragma control sched=1

// ── eWeatherEffect::Write(cFile &) const @ 0x000606D0 ──
void eWeatherEffect::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    this->eGeom::Write(file);
    wb.End();
}

// ── eWeatherEffect::Read(cFile &, cMemPool *) @ 0x0006071C ──
int eWeatherEffect::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 && this->eGeom::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── eWeatherEffect::New(cMemPool *, cBase *) static @ 0x0020754C ──
cBase *eWeatherEffect::New(cMemPool *pool, cBase *parent) {
    eWeatherEffect *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eWeatherEffect *obj = (eWeatherEffect *)rec->fn(base, 0xA0, 0x10, 0, 0);
    if (obj != 0) {
        eWeatherEffect__eWeatherEffect_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── eWeatherEffect::GetType(void) const @ 0x002075c8 ──
const cType *eWeatherEffect::GetType(void) const {
    if (!type_eWeatherEffect) {
        if (!type_eGeom) {
            if (!type_cBase) {
                const char *name = (const char *)0x36CD74;
                const char *desc = (const char *)0x36CD7C;
                __asm__ volatile("" : "+r"(name), "+r"(desc));
                type_cBase = cType::InitializeType(name, desc, 1, 0, 0, 0, 0, 0);
            }
            type_eGeom = cType::InitializeType(0, 0, 0x16, type_cBase, 0, 0, 0, 0);
        }
        __asm__ volatile("" ::: "memory");
        const cType *parentType = type_eGeom;
        cBase *(*factory)(cMemPool *, cBase *) =
            (cBase *(*)(cMemPool *, cBase *))0x20754C;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        type_eWeatherEffect =
            cType::InitializeType(0, 0, 0x192, parentType, factory, 0, 0, 0);
    }
    return type_eWeatherEffect;
}
