// ODR-WARNING: local redeclaration for a split-TU method addition.
// Keep this class shape limited to methods referenced in this translation unit.

class cBase;
class cMemPool;
class cType;

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cType {
public:
    char _pad[0x1C];
    const cType *mParent;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcMap {
public:
    gcMap &operator=(const gcMap &);
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
};

struct DispatchEntry {
    short offset;
    short _pad;
    cType *(*fn)(void *, short, void *);
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00040C90;
extern cType *D_00099AE0;
extern cType *D_00099B04;

// 0x00247204 -- gcMap::AssignCopy(const cBase *)
void gcMap::AssignCopy(const cBase *base) {
    const gcMap *other = 0;

    if (base != 0) {
        if (D_00099B04 == 0) {
            if (D_00099AE0 == 0) {
                if (D_00040C90 == 0) {
                    if (D_000385E4 == 0) {
                        if (D_000385E0 == 0) {
                            if (D_000385DC == 0) {
                                D_000385DC = cType::InitializeType(
                                    (const char *)0x36D894,
                                    (const char *)0x36D89C,
                                    1, 0, 0, 0, 0, 0);
                            }
                            D_000385E0 = cType::InitializeType(
                                0, 0, 2, D_000385DC,
                                &cNamed::New, 0, 0, 0);
                        }
                        D_000385E4 = cType::InitializeType(
                            0, 0, 3, D_000385E0, 0, 0, 0, 0);
                    }
                    D_00040C90 = cType::InitializeType(
                        0, 0, 5, D_000385E4, 0, 0, 0, 0);
                }
                D_00099AE0 = cType::InitializeType(
                    0, 0, 0xB3, D_00040C90, 0, 0, 0, 0);
            }
            D_00099B04 = cType::InitializeType(
                0, 0, 0x65, D_00099AE0, &gcMap::New,
                (const char *)0x36D938, (const char *)0x36D940, 1);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_00099B04;
        DispatchEntry *entry = (DispatchEntry *)((char *)classDesc + 8);
        short offset = entry->offset;
        cType *(*fn)(void *, short, void *) = entry->fn;
        cType *type = fn((char *)base + offset, offset, fn);
        int isValid;

        if (target != 0) {
            goto have_target;
        }
        isValid = 0;
        goto cast_done;

have_target:
        if (type != 0) {
loop_cast:
            if (type == target) {
                isValid = 1;
            } else {
                type = (cType *)type->mParent;
                if (type != 0) {
                    goto loop_cast;
                }
                goto invalid_cast;
            }
        } else {
invalid_cast:
            isValid = 0;
        }

cast_done:
        if (isValid != 0) {
            other = (const gcMap *)base;
        }
    }
    operator=(*other);
}
