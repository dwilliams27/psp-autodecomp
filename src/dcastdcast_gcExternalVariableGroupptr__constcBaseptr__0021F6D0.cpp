// ── dcast<gcExternalVariableGroup *>(const cBase *) @ 0x0021f6d0 ──
// RTTI downcast (gcAll_psp.obj). Lazily register gcExternalVariableGroup's cType
// via a 3-level InitializeType cascade, then walk the object's dynamic type chain
// for the registered type. Mirrors the matched gcVariableGroup 3-level shape.
// Constants read from this function's OWN 0x0021f6d0 disasm:
//   inner  D_000385DC : InitializeType(0x36D894, 0x36D89C, 1, 0, 0, 0,0,0)
//   middle D_00040C94 : InitializeType(0, 0, 0x4,   D_000385DC, 0, 0,0,0)
//   outer  D_000998D0 : InitializeType(0, 0, 0x167, D_00040C94,
//                                       &gcExternalVariableGroup::New, 0,0, 0x8)
// gcExternalVariableGroup::New @ 0x2379FC.

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

class gcExternalVariableGroup {
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
extern cType *D_000998D0;

template <class T>
T dcast(const cBase *obj)
{
    T result = 0;
    if (obj != 0) {
        if (D_000998D0 == 0) {
            if (D_00040C94 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_00040C94 = cType::InitializeType(0, 0, 0x4, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_000998D0 = cType::InitializeType(0, 0, 0x167, D_00040C94,
                                               &gcExternalVariableGroup::New, 0, 0, 0x8);
        }
        cTypeDispatch *disp = &((cBaseLayout *)obj)->info->dispatch;
        cTypeNode *registered = (cTypeNode *)D_000998D0;
        short adjust = disp->adjust;
        const cType *(*getType)(const cBase *) = disp->getType;
        cTypeNode *t = (cTypeNode *)getType((const cBase *)((char *)obj + adjust));
        int found;

        if (registered != 0) {
            goto have_target;
        }
        found = 0;
        goto cast_done;

have_target:
        if (t != 0) {
loop_cast:
            if (t == registered) {
                found = 1;
            } else {
                t = t->parent;
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

template gcExternalVariableGroup *dcast<gcExternalVariableGroup *>(const cBase *obj);
