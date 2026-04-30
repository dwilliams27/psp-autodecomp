class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(float);
    void End(void);
};

class cBaseArray {
public:
    void Write(cWriteBlock &) const;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

struct eCameraEffectLayout {
    char pad[0x4C];
    bool mField4C;
    bool mField4D;
    bool mField4E;
    char pad4F;
    float mField50;
    float mField54;
    float mField58;
    float mField5C;
};

class cObject {
public:
    cObject(cBase *);
    void Write(cFile &) const;
};

struct eCameraEffectPoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct eCameraEffectAllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

extern char eCameraEffectvirtualtable[];
extern char eCameraEffectGeomvirtualtable[];

extern "C" void cObject_cObject(void *, cBase *);
extern "C" void eGeom_eGeom(void *, cBase *);

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00041008;

class eCameraEffect : public cObject {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

// 0x001e31f0 - eCameraEffect::New(cMemPool *, cBase *) static
cBase *eCameraEffect::New(cMemPool *pool, cBase *parent) {
    eCameraEffect *result = 0;
    void *block = ((void **)pool)[9];
    char *allocTable = ((eCameraEffectPoolBlock *)block)->allocTable;
    eCameraEffectAllocEntry *entry = (eCameraEffectAllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    eCameraEffect *obj = (eCameraEffect *)entry->fn(base, 0xF0, 0x10, 0, 0);
    if (obj != 0) {
        cObject_cObject(obj, parent);
        *(void **)((char *)obj + 4) = eCameraEffectvirtualtable;
        *(int *)((char *)obj + 0x44) = 0;
        *(void **)((char *)obj + 0x48) = obj;
        *(unsigned char *)((char *)obj + 0x4C) = 0;
        *(unsigned char *)((char *)obj + 0x4D) = 0;
        *(unsigned char *)((char *)obj + 0x4E) = 0;
        *(float *)((char *)obj + 0x50) = 0.0f;
        *(float *)((char *)obj + 0x54) = 1.0f;
        *(float *)((char *)obj + 0x58) = 10.0f;
        *(float *)((char *)obj + 0x5C) = 30.0f;
        eGeom_eGeom((char *)obj + 0x60, (cBase *)obj);
        *(void **)((char *)obj + 0x64) = eCameraEffectGeomvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

// 0x0002d258 - eCameraEffect::Write(cFile &) const
void eCameraEffect::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    cObject::Write(file);
    ((const cBaseArray *)((const char *)this + 0x44))->Write(wb);
    const eCameraEffectLayout *self = (const eCameraEffectLayout *)this;
    wb.Write(self->mField4C);
    wb.Write(self->mField4D);
    wb.Write(self->mField4E);
    wb.Write(self->mField50);
    wb.Write(self->mField54);
    wb.Write(self->mField58);
    wb.Write(self->mField5C);
    wb.End();
}

const cType *eCameraEffect::GetType(void) const {
    if (D_00041008 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                       (const char *)0x36CD7C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                                   (cBase *(*)(cMemPool *, cBase *))&cNamed::New,
                                                   0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                               0, 0, 0, 0);
        }
        D_00041008 = cType::InitializeType(0, 0, 0x61, D_000385E4,
                                           &eCameraEffect::New,
                                           (const char *)0x36CD94,
                                           (const char *)0x36CDA4, 5);
    }
    return D_00041008;
}
