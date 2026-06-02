// gcMap::DynamicallyLoadedObjectsSetToLoadDiffer(void)  @ 0x000F6370  492B
// obj: gcAll_psp.obj
//
// Lazy 4-level cType::InitializeType registration guard (same idiom as
// gcValEntityIsValid::GetType), then a cFactory::FindGroup type query and a
// circular linked-list walk over the group's objects.  Returns whether the
// number of currently-set dynamic-load objects differs from gNumDynObjLoad.

class cMemPool;
class cBase;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

struct gcGuid {
    int a;
    int b;
};

struct gcObject {
    char _p00[0x28];
    unsigned short mFlags;   // 0x28
    char _p2a[0x34 - 0x2a];
    gcObject *mNext;         // 0x34
};

struct gcGroup {
    char _p00[0x0c];
    gcObject *mHead;         // 0x0c
};

class cFactory {
public:
    gcGroup *FindGroup(const cType *);
};

class gcMap : public cFactory {
public:
    int DynamicallyLoadedObjectsSetToLoadDiffer(void);
    int IsDynamicallyLoadedObjectSetToLoad(const gcGuid &) const;
};

// Type-init constant args (masked relocations / absolute immediates).
extern const char gcMap_dynobj_name[];   // 0x0036D894
extern const char gcMap_dynobj_desc[];   // 0x0036D89C
extern cBase *gcMap_dynobj_new(cMemPool *, cBase *); // 0x001C3C58 (level-2 fn)
extern cBase *gcMap_dynobj_new2(cMemPool *, cBase *);// 0x0026882C (level-4 fn)
extern const char gcMap_dynobj_s1[];     // 0x0036D9B8
extern const char gcMap_dynobj_s2[];     // 0x0036D9C8

extern cType *gType_base;          // 0x000385DC
extern cType *gType_expression;    // 0x000385E0
extern cType *gType_value;         // 0x000385E4
extern cType *gType_dynobj;        // 0x0009F448

extern int gNumDynObjLoad;         // 0x0037D7F8

int gcMap::DynamicallyLoadedObjectsSetToLoadDiffer(void) {
    unsigned int count = 0;

    if (!gType_dynobj) {
        if (!gType_value) {
            if (!gType_expression) {
                if (!gType_base) {
                    gType_base = cType::InitializeType(
                        gcMap_dynobj_name, gcMap_dynobj_desc, 1, 0, 0, 0, 0, 0);
                }
                gType_expression = cType::InitializeType(
                    0, 0, 2, gType_base, gcMap_dynobj_new, 0, 0, 0);
            }
            gType_value = cType::InitializeType(
                0, 0, 3, gType_expression, 0, 0, 0, 0);
        }
        gType_dynobj = cType::InitializeType(
            0, 0, 0x8E, gType_value, gcMap_dynobj_new2,
            gcMap_dynobj_s1, gcMap_dynobj_s2, 5);
    }

    gcGroup *group = FindGroup(gType_dynobj);
    gcObject *o = group->mHead;
    while (o) {
        unsigned short flags = o->mFlags;
        if ((unsigned char)(bool)(flags & 0x600) && (unsigned char)(bool)(flags & 0x40)) {
            if (IsDynamicallyLoadedObjectSetToLoad(*(const gcGuid *)((char *)o + 0x20))) {
                count += 1;
            } else {
                return 1;
            }
        }
        gcObject *next = o->mNext;
        o = 0;
        if (next != group->mHead) {
            o = next;
        }
    }

    int n = gNumDynObjLoad;
    return n != count;
}
