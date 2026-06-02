// dcast<gcEntityTemplateGroup *>(const cBase *) @ 0x0021f02c
// RTTI downcast: inline GetType type-init ladder (3-level cascade) + virtual
// GetType dispatch + parent-chain walk. Mirrors the matched 3-level dcast
// (eGeomTemplateGroup @ 001c8970), sharing the same parent (D_00040C94 ord 4)
// and base (D_000385DC ord 1, strings 0x36D894/0x36D89C). Leaf differs:
// global D_000998BC, ordinal 0x8F, &gcEntityTemplateGroup::New @ 0x236DF4, t3=8.

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

class gcEntityTemplateGroup {
public:
    static cBase *New(cMemPool *, cBase *);
};

template <class T> T dcast(const cBase *);

extern cType *D_000385DC;
extern cType *D_00040C94;
extern cType *D_000998BC;

// Class-descriptor dispatch entry at (classDesc + 8): a short offset followed
// by the runtime-type accessor function pointer.
struct DispatchEntry {
    short offset;
    short _pad;
    cType *(*fn)(const void *, short, void *);
};

template <class T>
T dcast(const cBase *obj) {
    T result = 0;
    if (obj != 0) {
        if (D_000998BC == 0) {
            if (D_00040C94 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_00040C94 = cType::InitializeType(0, 0, 4, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_000998BC = cType::InitializeType(0, 0, 0x8F, D_00040C94,
                                               &gcEntityTemplateGroup::New, 0, 0, 8);
        }
        void *classDesc = *(void **)((char *)obj + 4);
        cType *target = D_000998BC;
        DispatchEntry *entry = (DispatchEntry *)((char *)classDesc + 8);
        short off = entry->offset;
        cType *(*fn)(const void *, short, void *) = entry->fn;
        cType *t = fn((const char *)obj + off, off, (void *)fn);

        int matched;
        if (target == 0) {
            matched = 0;
        } else if (t != 0) {
            for (;;) {
                if (t == target) {
                    matched = 1;
                    break;
                }
                t = *(cType **)((char *)t + 0x1C);
                if (t == 0) {
                    goto nomatch;
                }
            }
        } else {
        nomatch:
            matched = 0;
        }
        if (matched != 0) {
            result = (T)obj;
        }
    }
    return result;
}

template gcEntityTemplateGroup *dcast<gcEntityTemplateGroup *>(const cBase *);
