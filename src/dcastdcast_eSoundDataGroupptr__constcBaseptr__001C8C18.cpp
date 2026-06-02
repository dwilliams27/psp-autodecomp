// ── dcast<eSoundDataGroup *>(const cBase *) @ 0x001c8c18 ──
// RTTI downcast: register eSoundDataGroup's cType lazily via a 3-level
// type-init cascade (base eAll type D_000385DC ord 1, middle group base
// D_00040C94 ord 4, derived eSoundDataGroup D_00040E30 ord 0x27 with
// &eSoundDataGroup::New 5th arg and 8 as the 8th arg), then walk the object's
// dynamic type chain (t = t->mParent) looking for the registered type.
// Mirrors the matched dcast<eSound *>/dcast<eBspTree *> dispatch shape.

class cBase;
class cMemPool;
class cType;
class eSoundDataGroup;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class eSoundDataGroup {
public:
    static cBase *New(cMemPool *, cBase *);
};

struct DispatchEntry {
    short offset;
    short pad;
    cType *(*fn)(const void *, short, void *);
};

extern cType *D_000385DC;
extern cType *D_00040C94;
extern cType *D_00040E30;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    T result = 0;

    if (base != 0) {
        if (D_00040E30 == 0) {
            if (D_00040C94 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                       (const char *)0x36CD7C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_00040C94 = cType::InitializeType(0, 0, 4, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_00040E30 = cType::InitializeType(0, 0, 0x27, D_00040C94,
                                               &eSoundDataGroup::New,
                                               0, 0, 8);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_00040E30;
        DispatchEntry *entry = (DispatchEntry *)((char *)classDesc + 8);
        short off = entry->offset;
        cType *(*fn)(const void *, short, void *) = entry->fn;
        cType *t = fn((const char *)base + off, off, (void *)fn);

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
            result = (T)base;
        }
    }

    return result;
}

template eSoundDataGroup *dcast<eSoundDataGroup *>(const cBase *base);
