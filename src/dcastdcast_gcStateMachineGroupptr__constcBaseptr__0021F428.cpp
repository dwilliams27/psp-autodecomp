// dcast<gcStateMachineGroup *>(const cBase *) @ 0x0021f428
// RTTI downcast: lazy cType registration cascade (3-tier hierarchy) + dynamic-type
// chain walk. Mirrors the matched gcEntityTemplateGroup/gcViewport dcast pattern; the
// gcAll-specific cache slots, type ordinals and &New read from this fn's own disasm.

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

class gcStateMachineGroup {
public:
    static cBase *New(cMemPool *, cBase *);
};

// classdesc thunk record at obj->[4] + 8: { short offset; ptr GetType(thisAdj); }
struct ClassDesc {
    short offset;
    short _pad;
    const cType *(*getType)(const void *);
};

struct cBaseLayout {
    void *_vtbl;       // +0
    ClassDesc *desc;   // +4
};

extern cType *D_000385DC;   // base type cache  (lui 0x4, -31268)
extern cType *D_00040C94;   // mid type cache   (lui 0x4,  3220)
extern cType *D_000998C8;   // leaf type cache  (lui 0xa, -26424)

template <class T> T dcast(const cBase *);

template <class T> T dcast(const cBase *obj) {
    T result = 0;
    if (obj != 0) {
        if (D_000998C8 == 0) {
            if (D_00040C94 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_00040C94 = cType::InitializeType(0, 0, 0x4, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_000998C8 = cType::InitializeType(0, 0, 0xB7, D_00040C94,
                                               &gcStateMachineGroup::New,
                                               0, 0, 0x8);
        }

        const cType *target = D_000998C8;

        ClassDesc *desc = (ClassDesc *)((char *)((cBaseLayout *)obj)->desc + 8);
        short adj = desc->offset;
        const cType *(*fn)(const void *) = desc->getType;
        const cType *t = fn((const char *)obj + adj);

        int found;
        if (target != 0) {
            goto have_target;
        }
        found = 0;
        goto cast_done;

have_target:
        if (t != 0) {
loop_cast:
            if (t == target) {
                found = 1;
            } else {
                t = *(const cType **)((char *)t + 0x1C);
                if (t != 0) {
                    goto loop_cast;
                }
                goto invalid_cast;
            }
        } else {
invalid_cast:
            found = 0;
        }

cast_done:
        if (found != 0) {
            result = (T)obj;
        }
    }
    return result;
}

template gcStateMachineGroup *dcast<gcStateMachineGroup *>(const cBase *);
