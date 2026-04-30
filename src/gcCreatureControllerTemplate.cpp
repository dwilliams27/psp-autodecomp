// gcCreatureControllerTemplate.cpp — gcAll_psp.obj
//
// Functions:
//   gcCreatureControllerTemplate::Write(cFile &) const          @ 0x0013C92C  (76B)
//   gcCreatureControllerTemplate::Read(cFile &, cMemPool *)     @ 0x0013C978 (188B)
//   gcCreatureControllerTemplate::GetType(void) const           @ 0x00293FE4 (212B)
//
// gcCreatureControllerTemplate inherits from gcEntityControllerTemplate.
// Type chain: cBase (id 1) -> gcEntityControllerTemplate (id 0x9A) ->
// gcCreatureControllerTemplate (id 0xB8).

class cBase;
class cFile;
class cType;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

extern const char gcCreatureControllerTemplate_typeName[];  // 0x36D894 — "cBase"
extern const char gcCreatureControllerTemplate_typeDesc[];  // 0x36D89C — "Base"

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

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

class cBaseArray {
public:
    void RemoveAll(void);
};

void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void free(void *);

class gcEntityControllerTemplate {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcCreatureControllerTemplate : public gcEntityControllerTemplate {
public:
    ~gcCreatureControllerTemplate();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    const cType *GetType(void) const;
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

inline void operator delete(void *p) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(p);
    if (pool != 0) {
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    } else {
        free(p);
    }
}

extern char gcEntityControllerTemplate_dtor_classdesc[];
extern char cBase_dtor_classdesc[];

// Static type-cache slots — addresses 0x385DC, 0x9A400, 0x9F5A4 in the binary.
// Relocation masking in compare_func handles distinct addresses.
static cType *type_gcCreature_base;
static cType *type_gcCreature_mid;
static cType *type_gcCreature;

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0ocgcCreatureControllerTemplatedtv, 0xcc\n");

// -- gcCreatureControllerTemplate::~gcCreatureControllerTemplate(void) --
gcCreatureControllerTemplate::~gcCreatureControllerTemplate() {
    *(char **)((char *)this + 4) = gcEntityControllerTemplate_dtor_classdesc;
    cBaseArray *arr1 = (cBaseArray *)((char *)this + 0x1C);
    cBaseArray *arr0 = (cBaseArray *)((char *)this + 0x08);
    if (arr1 != 0) {
        arr1->RemoveAll();
    }
    if (arr0 != 0) {
        arr0->RemoveAll();
    }
    *(char **)((char *)this + 4) = cBase_dtor_classdesc;
}

// ── gcCreatureControllerTemplate::Write(cFile &) const ──
void gcCreatureControllerTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const gcEntityControllerTemplate *)this)->Write(file);
    wb.End();
}

// ── gcCreatureControllerTemplate::Read(cFile &, cMemPool *) ──
int gcCreatureControllerTemplate::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 2, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 2 && this->gcEntityControllerTemplate::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── gcCreatureControllerTemplate::GetType(void) const ──
const cType *gcCreatureControllerTemplate::GetType(void) const {
    if (!type_gcCreature) {
        if (!type_gcCreature_mid) {
            if (!type_gcCreature_base) {
                type_gcCreature_base = cType::InitializeType(
                    gcCreatureControllerTemplate_typeName,
                    gcCreatureControllerTemplate_typeDesc,
                    1, 0, 0, 0, 0, 0);
            }
            type_gcCreature_mid = cType::InitializeType(
                0, 0, 0x9A, type_gcCreature_base, 0, 0, 0, 0);
        }
        type_gcCreature = cType::InitializeType(
            0, 0, 0xB8, type_gcCreature_mid, 0, 0, 0, 0);
    }
    return type_gcCreature;
}
