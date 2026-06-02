// ── dcast<gcTableColumnByte *>(const cBase *) @ 0x00224594 (340B, gcAll_psp.obj) ──
// RTTI downcast. Lazily register gcTableColumnByte's cType via a 3-level
// InitializeType cascade, then walk the object's dynamic type chain for the
// registered type. 3-level shape (mirrors matched gcStringSet @ 0x00221d4c).
//   inner  D_000385DC : InitializeType(0x36D894, 0x36D89C, 1, 0, 0, 0,0,0)
//   middle D_0009F478 : InitializeType(0,0, 0x241, D_000385DC, 0, 0,0,0)
//   outer  D_0009F47C : InitializeType(0,0, 0x242, D_0009F478,
//                                      &gcTableColumnByte::New, 0,0,0)
// gcTableColumnByte::New @ 0x00270F64.

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

class gcTableColumnByte {
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
extern cType *D_0009F478;
extern cType *D_0009F47C;

template <class T>
T dcast(const cBase *obj)
{
    T result = 0;
    if (obj != 0) {
        if (D_0009F47C == 0) {
            if (D_0009F478 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_0009F478 = cType::InitializeType(0, 0, 0x241, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_0009F47C = cType::InitializeType(0, 0, 0x242, D_0009F478,
                                               &gcTableColumnByte::New, 0, 0, 0);
        }
        cTypeDispatch *disp = &((cBaseLayout *)obj)->info->dispatch;
        cType *registered = D_0009F47C;
        short adjust = disp->adjust;
        const cType *(*getType)(const cBase *) = disp->getType;
        cTypeNode *t = (cTypeNode *)getType((const cBase *)((char *)obj + adjust));
        int found;
        if (registered == 0) {
            found = 0;
        } else if (t != 0) {
            for (;;) {
                if (t == (cTypeNode *)registered) {
                    found = 1;
                    break;
                }
                t = t->parent;
                if (t == 0) {
                    goto nomatch;
                }
            }
        } else {
        nomatch:
            found = 0;
        }
        if (found != 0) {
            result = (T)obj;
        }
    }
    return result;
}

template gcTableColumnByte *dcast<gcTableColumnByte *>(const cBase *obj);
