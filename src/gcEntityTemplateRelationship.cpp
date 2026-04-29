// gcEntityTemplateRelationship.cpp - gcAll_psp.obj
//
// Functions in this file:
//   0x0031b16c gcEntityTemplateRelationship::New(cMemPool *, cBase *) static
//   0x0031b238 gcEntityTemplateRelationship::GetType(void) const
//   0x0015354c gcEntityTemplateRelationship::Write(cFile &) const

class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    cFile *_file;
    int _pos;
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

class gcObjectRelationship {
public:
    void Write(cFile &) const;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

struct PoolBlock {
    char _pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcEntityTemplateRelationship : public gcObjectRelationship {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

extern cType *D_000385DC;
static cType *type_gcObjectRelationship;
static cType *type_gcEntityTemplateRelationship;

cBase *gcEntityTemplateRelationship::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcEntityTemplateRelationship *result = 0;
    gcEntityTemplateRelationship *obj =
        (gcEntityTemplateRelationship *)entry->fn(base, 0x28, 4, 0, 0);
    if (obj != 0) {
        ((int *)obj)[1] = 0x37E6A8;
        ((cBase **)obj)[0] = parent;
        ((int *)obj)[1] = 0x388928;
        ((int *)obj)[3] = 0x37E6A8;
        ((gcEntityTemplateRelationship **)obj)[2] = obj;
        ((int *)obj)[3] = 0x388568;
        ((unsigned char *)obj)[0x10] = 1;
        ((unsigned char *)obj)[0x11] = 0;
        ((int *)obj)[5] = 0;
        ((int *)obj)[6] = 0;
        ((int *)obj)[7] = (int)((unsigned int)obj | 1);
        ((int *)obj)[1] = 0x38C820;
        ((int *)obj)[8] = 7;
        ((int *)obj)[9] = 0;
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcEntityTemplateRelationship::GetType(void) const {
    if (!type_gcEntityTemplateRelationship) {
        if (!type_gcObjectRelationship) {
            if (!D_000385DC) {
                D_000385DC = cType::InitializeType(
                    (const char *)0x36D894, (const char *)0x36D89C,
                    1, 0, 0, 0, 0, 0);
            }
            type_gcObjectRelationship = cType::InitializeType(
                0, 0, 0x131, D_000385DC, 0, 0, 0, 0);
        }
        type_gcEntityTemplateRelationship = cType::InitializeType(
            0, 0, 0x206, type_gcObjectRelationship,
            (cBase *(*)(cMemPool *, cBase *))&gcEntityTemplateRelationship::New,
            0, 0, 0);
    }
    return type_gcEntityTemplateRelationship;
}

void gcEntityTemplateRelationship::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcObjectRelationship::Write(file);

    cWriteBlock inner(*wb._file, 3);
    inner.Write(((int *)this)[8]);
    ((const cHandle *)((const char *)this + 0x24))->Write(inner);
    inner.End();
    wb.End();
}
