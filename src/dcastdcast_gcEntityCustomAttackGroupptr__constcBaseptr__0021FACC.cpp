// dcast<gcEntityCustomAttackGroup*>(const cBase*) — three-level InitializeType RTTI cascade.
//   inner  D_000385DC : InitializeType(0x36D894, 0x36D89C, 1, 0, 0, 0,0,0)
//   middle D_00040C94 : InitializeType(0, 0, 4, D_000385DC, 0, 0,0,0)
//   outer  D_000998DC : InitializeType(0, 0, 0x1CE, D_00040C94, &gcEntityCustomAttackGroup::New, 0,0,8)
// gcEntityCustomAttackGroup::New referenced via the class static (0x238134).
// Mirrors the matched dcast<gcEntityGroup*> three-level shape; constants read from
// this function's own asm/0.s entry guard (outer slot 0x998DC, ordinal 0x1CE, 8th arg 8).
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

class gcEntityCustomAttackGroup {
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
extern cType *D_000998DC;

template <class T>
T dcast(const cBase *obj)
{
    T result = 0;
    if (obj != 0) {
        if (D_000998DC == 0) {
            if (D_00040C94 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_00040C94 = cType::InitializeType(0, 0, 4, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_000998DC = cType::InitializeType(0, 0, 0x1CE, D_00040C94,
                                               &gcEntityCustomAttackGroup::New, 0, 0, 8);
        }
        cTypeDispatch *disp = &((cBaseLayout *)obj)->info->dispatch;
        cType *registered = D_000998DC;
        short adjust = disp->adjust;
        const cType *(*getType)(const cBase *) = disp->getType;
        cTypeNode *t = (cTypeNode *)getType((const cBase *)((char *)obj + adjust));
        int found;
        if (registered == 0) {
            found = 0;
        } else {
            while (t != 0) {
                if (t == (cTypeNode *)registered) {
                    found = 1;
                    goto done;
                }
                t = t->parent;
            }
            found = 0;
        }
    done:
        if (found) {
            result = (T)obj;
        }
    }
    return result;
}

template gcEntityCustomAttackGroup *dcast<gcEntityCustomAttackGroup *>(const cBase *obj);
