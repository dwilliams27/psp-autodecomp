// dcast<gcStringTableGroup *>(const cBase *) @ 0x0021ec30
// RTTI downcast (gcAll_psp.obj). 3-level lazy cType registration cascade
// (vs the 2-level gcRegionSet/gcViewport shape), then a dynamic-type chain walk.
// Constants read from this fn's own disassembly:
//   inner  D_000385DC : InitializeType(0x36D894, 0x36D89C, 1, 0, 0, 0, 0, 0)
//   middle D_00040C94 : InitializeType(0, 0, 4,    D_000385DC, 0,    0, 0, 0)
//   outer  D_000998B0 : InitializeType(0, 0, 0x88, D_00040C94, &New, 0, 0, 8)
// gcStringTableGroup::New @ 0x2366BC. Outer's trailing uint arg is 8.

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

class gcStringTableGroup {
public:
    static cBase *New(cMemPool *, cBase *);
};

struct cTypeDispatch {
    short adjust;
    char pad[2];
    const cType *(*getType)(const cBase *);
};

struct cTypeInfo {
    char pad8[8];
    cTypeDispatch dispatch;
};

struct cBaseLayout {
    char pad4[4];
    cTypeInfo *info;
};

struct cTypeNode {
    char pad[0x1C];
    cTypeNode *parent;
};

extern cType *D_000385DC;
extern cType *D_00040C94;
extern cType *D_000998B0;

template <class T>
T dcast(const cBase *obj)
{
    T result = 0;
    if (obj != 0) {
        if (D_000998B0 == 0) {
            if (D_00040C94 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_00040C94 = cType::InitializeType(0, 0, 4, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_000998B0 = cType::InitializeType(0, 0, 0x88, D_00040C94,
                                               &gcStringTableGroup::New, 0, 0, 8);
        }
        cTypeDispatch *disp = &((cBaseLayout *)obj)->info->dispatch;
        cType *registered = D_000998B0;
        short adjust = disp->adjust;
        const cType *(*getType)(const cBase *) = disp->getType;
        cTypeNode *t = (cTypeNode *)getType((const cBase *)((char *)obj + adjust));
        int found;
        if (registered != 0) {
            goto have_target;
        }
        found = 0;
        goto done_check;

have_target:
        if (t != 0) {
loop_walk:
            if (t == (cTypeNode *)registered) {
                found = 1;
            } else {
                t = t->parent;
                if (t != 0) {
                    goto loop_walk;
                }
                goto no_match;
            }
        } else {
no_match:
            found = 0;
        }

done_check:
        if (found != 0) {
            result = (T)obj;
        }
    }
    return result;
}

template gcStringTableGroup *dcast<gcStringTableGroup *>(const cBase *obj);
