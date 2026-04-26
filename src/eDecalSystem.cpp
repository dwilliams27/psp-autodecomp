class cBase;
class cFile;
class cMemPool;
class cType;
class eGeom;
class eDecalSystem;

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

extern const char eDecalSystem_cBase_name[];   // 0x36CD74 = "cBase"
extern const char eDecalSystem_cBase_desc[];   // 0x36CD7C = "Base"

static cType *type_cBase;
static cType *type_eGeom;
static cType *type_eDecalSystem;

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
