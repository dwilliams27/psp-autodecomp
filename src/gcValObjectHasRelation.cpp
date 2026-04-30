class cBase;
class cMemPool;
class cType;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

extern char cBaseclassdesc[];
extern char D_000098C0[];
extern char gcDesiredEnumerationEntryvirtualtable[];

struct AllocBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcValObjectHasRelation {
public:
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_value asm("D_0009F3E8");
static cType *type_gcValObjectHasRelation asm("D_0009F8C0");

// ── gcValObjectHasRelation::New(cMemPool *, cBase *) static @ 0x00355768 ──
cBase *gcValObjectHasRelation::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((AllocBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValObjectHasRelation *result = 0;
    gcValObjectHasRelation *obj =
        (gcValObjectHasRelation *)entry->fn(base, 0x28, 4, 0, 0);
    if (obj != 0) {
        *(void **)((char *)obj + 4) = cBaseclassdesc;
        *(cBase **)((char *)obj + 0) = parent;
        *(void **)((char *)obj + 4) = D_000098C0;
        int flagged = (int)obj | 1;
        *(void **)((char *)obj + 0x10) = cBaseclassdesc;
        *(int *)((char *)obj + 8) = flagged;
        *(gcValObjectHasRelation **)((char *)obj + 0xC) = obj;
        *(void **)((char *)obj + 0x10) = gcDesiredEnumerationEntryvirtualtable;
        *(unsigned char *)((char *)obj + 0x14) = 1;
        *(unsigned char *)((char *)obj + 0x15) = 0;
        *(int *)((char *)obj + 0x18) = 0;
        *(int *)((char *)obj + 0x1C) = 0;
        *(int *)((char *)obj + 0x20) = flagged;
        *(unsigned char *)((char *)obj + 0x24) = 0;
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcValObjectHasRelation::GetType(void) const {
    if (!type_gcValObjectHasRelation) {
        if (!type_value) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType((const char *)0x36D894,
                                                      (const char *)0x36D89C,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base,
                                                        0, 0, 0, 0);
            }
            type_value = cType::InitializeType(0, 0, 0x6C, type_expression,
                                               0, 0, 0, 0x80);
        }
        type_gcValObjectHasRelation =
            cType::InitializeType(0, 0, 0x140, type_value,
                                  gcValObjectHasRelation::New, 0, 0, 0);
    }
    return type_gcValObjectHasRelation;
}
