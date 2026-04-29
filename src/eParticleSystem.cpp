inline void *operator new(unsigned int, void *p) { return p; }

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

    eDynamicGeom(cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class eParticleSystem : public eDynamicGeom {
public:
    eParticleSystem(cBase *);
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    const cType *GetType(void) const;
};

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

extern char eParticleSystemvirtualtable[];
extern cType *D_000385DC;
extern cType *D_00040FF4;
extern cType *D_000469C0;
extern cType *D_00046C44;

#pragma control sched=1

// ── eParticleSystem::eParticleSystem(cBase *) @ 0x0007b850 ──
eParticleSystem::eParticleSystem(cBase *parent) : eDynamicGeom(parent) {
    *(void **)((char *)this + 4) = eParticleSystemvirtualtable;
    *(int *)((char *)this + 0xF0) = 0;
    *(int *)((char *)this + 0x100) = 0;
    *(int *)((char *)this + 0x104) = 0;
    *(int *)((char *)this + 0x108) = 0;
    *(int *)((char *)this + 0x10C) = 0;
    *(int *)((char *)this + 0x110) = 0;
}

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
        new (obj) eParticleSystem(parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── eParticleSystem::GetType(void) const @ 0x00214d04 ──
const cType *eParticleSystem::GetType(void) const {
    __asm__ volatile("" ::: "memory");
    if (D_00046C44 == 0) {
        if (D_000469C0 == 0) {
            if (D_00040FF4 == 0) {
                if (D_000385DC == 0) {
                    const char *name = (const char *)0x36CD74;
                    const char *desc = (const char *)0x36CD7C;
                    __asm__ volatile("" : "+r"(name), "+r"(desc));
                    D_000385DC = cType::InitializeType(name, desc, 1,
                                                       0, 0, 0, 0, 0);
                }
                D_00040FF4 = cType::InitializeType(0, 0, 0x16, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_000469C0 = cType::InitializeType(0, 0, 0x17, D_00040FF4,
                                               0, 0, 0, 0);
        }
        const cType *parentType = D_000469C0;
        __asm__ volatile("" : "+r"(parentType));
        __asm__ volatile("" ::: "memory");
        cBase *(*factory)(cMemPool *, cBase *) = eParticleSystem::New;
        __asm__ volatile("" : "+r"(factory));
        D_00046C44 = cType::InitializeType(0, 0, 0x30, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046C44;
}
