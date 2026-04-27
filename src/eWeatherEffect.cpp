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
};

extern "C" {
    void eWeatherEffect__eWeatherEffect_cBaseptr(void *self, cBase *parent);
}

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

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
