// dcast<eGeomTemplateGroup *>(const cBase *) @ 0x001c8970
// RTTI downcast: inline GetType type-init ladder (3-level cascade) + virtual
// GetType dispatch + parent-chain walk. Mirrors the matched inline-dcast
// dispatch shape (eBspTree / cConfigBase) with an extra cascade level.

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

class eGeomTemplateGroup {
public:
    static cBase *New(cMemPool *, cBase *);
};

template <class T> T dcast(const cBase *);

extern cType *D_000385DC;
extern cType *D_00040C94;
extern cType *D_00040E28;

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
        if (D_00040E28 == 0) {
            if (D_00040C94 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                       (const char *)0x36CD7C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_00040C94 = cType::InitializeType(0, 0, 4, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_00040E28 = cType::InitializeType(0, 0, 0x1F, D_00040C94,
                                               &eGeomTemplateGroup::New, 0, 0, 8);
        }
        void *classDesc = *(void **)((char *)obj + 4);
        cType *target = D_00040E28;
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

template eGeomTemplateGroup *dcast<eGeomTemplateGroup *>(const cBase *);
