class cBase;
class cFile;
class cMemPool;
class cType;
class eGeom;
class eDecalSystem;

typedef int v4sf_t __attribute__((mode(V4SF)));

template <class T>
class cArrayBase {
public:
    cArrayBase &operator=(const cArrayBase &);
};

class eDecalParticle;
class eDecalList;

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

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class eWorld {
public:
    enum eWorldGeomBin {
        eWorldGeomBin_Decal = 2
    };

    void RemoveBinGeom(eGeom *, eWorldGeomBin);
};

class eGeom {
public:
    int base;

    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class eDecalSystem : public eGeom {
public:
    char pad[0x60];
    eWorld *world;

    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    void OnRemovedFromWorld(void);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    const cType *GetType(void) const;
};

extern "C" {
    void eDecalSystem__eDecalSystem_cBaseptr(void *self, cBase *parent);
}

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

struct TypeNode {
    char pad[0x1C];
    cType *parent;
};

struct TypeDispatchSlot {
    short offset;
    short _pad;
    cType *(*getType)(void *);
};

extern const char eDecalSystem_cBase_name[];   // 0x36CD74 = "cBase"
extern const char eDecalSystem_cBase_desc[];   // 0x36CD7C = "Base"

static cType *type_cBase;
static cType *type_eGeom;
static cType *type_eDecalSystem;

#pragma control sched=2

// ── eDecalSystem::AssignCopy(const cBase *) @ 0x001E4E04 ──
void eDecalSystem::AssignCopy(const cBase *base) {
    const eDecalSystem *other = 0;

    if (base != 0) {
        if (type_eDecalSystem == 0) {
            if (type_eGeom == 0) {
                if (type_cBase == 0) {
                    type_cBase = cType::InitializeType(
                        eDecalSystem_cBase_name, eDecalSystem_cBase_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_eGeom = cType::InitializeType(0, 0, 0x16, type_cBase,
                                                   0, 0, 0, 0);
            }
            const cType *parentType = type_eGeom;
            cBase *(*factory)(cMemPool *, cBase *) =
                (cBase *(*)(cMemPool *, cBase *))&eDecalSystem::New;
            type_eDecalSystem = cType::InitializeType(0, 0, 0x199,
                                                      parentType, factory,
                                                      0, 0, 2);
        }

        cType *target = type_eDecalSystem;
        void *classDesc = *(void **)((const char *)base + 4);
        TypeDispatchSlot *slot = (TypeDispatchSlot *)((char *)classDesc + 8);
        short offset = slot->offset;
        cType *type = slot->getType((void *)((const char *)base + offset));
        int ok;

        if (target != 0) {
            goto have_target;
        }
        ok = 0;
        goto cast_done;

have_target:
        if (type != 0) {
loop:
            if (type == target) {
                ok = 1;
            } else {
                type = ((TypeNode *)type)->parent;
                if (type != 0) {
                    goto loop;
                }
                goto invalid;
            }
        } else {
invalid:
            ok = 0;
        }

cast_done:
        if (ok != 0) {
            other = (const eDecalSystem *)base;
        }
    }

    *(v4sf_t *)((char *)this + 0x40) =
        *(const v4sf_t *)((const char *)other + 0x40);
    *(v4sf_t *)((char *)this + 0x10) =
        *(const v4sf_t *)((const char *)other + 0x10);
    *(v4sf_t *)((char *)this + 0x20) =
        *(const v4sf_t *)((const char *)other + 0x20);
    *(v4sf_t *)((char *)this + 0x30) =
        *(const v4sf_t *)((const char *)other + 0x30);
    *(float *)((char *)this + 0x5C) =
        *(const float *)((const char *)other + 0x5C);
    *(v4sf_t *)((char *)this + 0x50) =
        *(const v4sf_t *)((const char *)other + 0x50);

    *(int *)((char *)this + 0x60) =
        *(const int *)((const char *)other + 0x60);
    *(int *)((char *)this + 0x64) =
        *(const int *)((const char *)other + 0x64);
    *(int *)((char *)this + 0x68) =
        *(const int *)((const char *)other + 0x68);
    *(int *)((char *)this + 0x6C) =
        *(const int *)((const char *)other + 0x6C);
    *(int *)((char *)this + 0x70) =
        *(const int *)((const char *)other + 0x70);
    *(float *)((char *)this + 0x74) =
        *(const float *)((const char *)other + 0x74);
    *(float *)((char *)this + 0x78) =
        *(const float *)((const char *)other + 0x78);
    *(int *)((char *)this + 0x7C) =
        *(const int *)((const char *)other + 0x7C);
    *(int *)((char *)this + 0x80) =
        *(const int *)((const char *)other + 0x80);
    *(int *)((char *)this + 0x84) =
        *(const int *)((const char *)other + 0x84);
    *(int *)((char *)this + 0x88) =
        *(const int *)((const char *)other + 0x88);

    *(unsigned char *)((char *)this + 0x8C) =
        *(const unsigned char *)((const char *)other + 0x8C);
    *(unsigned char *)((char *)this + 0x8D) =
        *(const unsigned char *)((const char *)other + 0x8D);

    ((cArrayBase<eDecalParticle> *)((char *)this + 0x90))->operator=(
        *(const cArrayBase<eDecalParticle> *)((const char *)other + 0x90));
    ((cArrayBase<eDecalList> *)((char *)this + 0x94))->operator=(
        *(const cArrayBase<eDecalList> *)((const char *)other + 0x94));

    *(int *)((char *)this + 0x98) =
        *(const int *)((const char *)other + 0x98);
    *(int *)((char *)this + 0x9C) =
        *(const int *)((const char *)other + 0x9C);
}

// ── eDecalSystem::OnRemovedFromWorld(void) @ 0x00033FE4 ──
void eDecalSystem::OnRemovedFromWorld(void) {
    world->RemoveBinGeom((eGeom *)this, eWorld::eWorldGeomBin_Decal);
}

// ── eDecalSystem::Write(cFile &) const @ 0x00032F40 ──
void eDecalSystem::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    this->eGeom::Write(file);
    wb.End();
}

// ── eDecalSystem::Read(cFile &, cMemPool *) @ 0x00032F8C ──
int eDecalSystem::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->eGeom::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── eDecalSystem::New(cMemPool *, cBase *) static @ 0x001E5020 ──
cBase *eDecalSystem::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eDecalSystem *result = 0;
    eDecalSystem *obj = (eDecalSystem *)rec->fn(base, 0xA0, 0x10, 0, 0);
    if (obj != 0) {
        eDecalSystem__eDecalSystem_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── eDecalSystem::GetType(void) const @ 0x001E509C ──
const cType *eDecalSystem::GetType(void) const {
    if (!type_eDecalSystem) {
        if (!type_eGeom) {
            if (!type_cBase) {
                type_cBase = cType::InitializeType(
                    eDecalSystem_cBase_name, eDecalSystem_cBase_desc,
                    1, 0, 0, 0, 0, 0);
            }
            type_eGeom = cType::InitializeType(
                0, 0, 0x16, type_cBase, 0, 0, 0, 0);
        }
        type_eDecalSystem = cType::InitializeType(
            0, 0, 0x199, type_eGeom,
            (cBase *(*)(cMemPool *, cBase *))&eDecalSystem::New,
            0, 0, 2);
    }
    return type_eDecalSystem;
}
