#include "gcDoObjectForEachRelationship.h"

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

void gcAction_gcAction(void *, cBase *);
void gcExpressionList_gcExpressionList(void *, void *);

extern char gcDoObjectForEachRelationshipvirtualtable[];
extern char gcDesiredEnumerationEntryvirtualtable[];
extern const char gcDoObjectForEachRelationship_base_name[] asm("D_0036D894");
extern const char gcDoObjectForEachRelationship_base_desc[] asm("D_0036D89C");

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoObjectForEachRelationship asm("D_0009F6B0");

int gcDoObjectForEachRelationship::GetMaxBranches(void) const {
    return 1;
}

cBase *gcDoObjectForEachRelationship::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoObjectForEachRelationship *result = 0;
    gcDoObjectForEachRelationship *obj =
        (gcDoObjectForEachRelationship *)entry->fn(base, 0x30, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoObjectForEachRelationshipvirtualtable;
        int self = (int)((unsigned int)obj | 1);
        ((int *)obj)[3] = self;
        ((int *)obj)[5] = 0x37E6A8;
        ((gcDoObjectForEachRelationship **)obj)[4] = obj;
        ((void **)obj)[5] = gcDesiredEnumerationEntryvirtualtable;
        *((char *)obj + 0x18) = 1;
        *((char *)obj + 0x19) = 0;
        ((int *)obj)[7] = 0;
        ((int *)obj)[8] = 0;
        ((int *)obj)[9] = self;
        gcExpressionList_gcExpressionList((char *)obj + 0x28, obj);
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDoObjectForEachRelationship::GetType(void) const {
    if (!type_gcDoObjectForEachRelationship) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoObjectForEachRelationship_base_name,
                        gcDoObjectForEachRelationship_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoObjectForEachRelationship = cType::InitializeType(
            0, 0, 0x1A4, type_action,
            gcDoObjectForEachRelationship::New, 0, 0, 0);
    }
    return type_gcDoObjectForEachRelationship;
}
