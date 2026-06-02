// ── dcast<gcShadowConfig *>(const cBase *) @ 0x0022c84c ──
// RTTI downcast (gcAll_psp.obj). Lazily register gcShadowConfig's cType via a
// 3-level InitializeType cascade, then walk the object's dynamic type chain
// looking for the registered type. Identical goto-form to the matched 280B
// dcast<gcViewport *> (00221AE0); only the cascade depth and constants differ.
//   inner  D_000385DC : InitializeType(0x36D894, 0x36D89C, 1, 0, 0, ...)
//   middle D_0009F43C : InitializeType(0,0, 0xA0,  D_000385DC,
//                                      &gcEntityGeomConfig::New, 0,0,0)
//   outer  D_0009F7BC : InitializeType(0,0, 0x1EA, D_0009F43C,
//                                      &gcShadowConfig::New, 0,0,0)
// gcShadowConfig::New @ 0x31FAB0, gcEntityGeomConfig::New @ 0x267384.

class cBase;
class cMemPool;
class cType;
class gcShadowConfig;

class cType {
public:
    char _pad[0x1C];
    const cType *mParent;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcEntityGeomConfig {
public:
    static cBase *New(cMemPool *, cBase *);
};

class gcShadowConfig {
public:
    static cBase *New(cMemPool *, cBase *);
};

struct DispatchEntry {
    short offset;
    short pad;
    cType *(*fn)(void *, short, void *);
};

extern cType *D_000385DC;
extern cType *D_0009F43C;
extern cType *D_0009F7BC;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_0009F7BC == 0) {
            if (D_0009F43C == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_0009F43C = cType::InitializeType(0, 0, 0xA0, D_000385DC,
                                                   &gcEntityGeomConfig::New, 0, 0, 0);
            }
            D_0009F7BC = cType::InitializeType(0, 0, 0x1EA, D_0009F43C,
                                               &gcShadowConfig::New, 0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009F7BC;
        DispatchEntry *entry = (DispatchEntry *)((char *)classDesc + 8);
        short offset = entry->offset;
        cType *(*fn)(void *, short, void *) = entry->fn;
        cType *type = fn((char *)base + offset, offset, (void *)fn);
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
            result = base;
        }
    }

    return (T)result;
}

template gcShadowConfig *dcast<gcShadowConfig *>(const cBase *base);
