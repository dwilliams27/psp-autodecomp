class cBase;
class cFile;
class cMemPool;
class cType;
class eDynamicGeom;
class eParticleSystem;

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

class eDynamicGeom {
public:
    int base;

    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class eParticleSystem : public eDynamicGeom {
public:
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

extern "C" {
    void eParticleSystem__eParticleSystem_cBaseptr(void *self, cBase *parent);
}

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

#pragma control sched=1

// ── eParticleSystem::Write(cFile &) const @ 0x0007b748 ──
void eParticleSystem::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    this->eDynamicGeom::Write(file);
    wb.End();
}

// ── eParticleSystem::Read(cFile &, cMemPool *) @ 0x0007b794 ──
int eParticleSystem::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 && this->eDynamicGeom::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── eParticleSystem::New(cMemPool *, cBase *) static @ 0x00214c88 ──
cBase *eParticleSystem::New(cMemPool *pool, cBase *parent) {
    eParticleSystem *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eParticleSystem *obj = (eParticleSystem *)rec->fn(base, 0x130, 0x10, 0, 0);
    if (obj != 0) {
        eParticleSystem__eParticleSystem_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
