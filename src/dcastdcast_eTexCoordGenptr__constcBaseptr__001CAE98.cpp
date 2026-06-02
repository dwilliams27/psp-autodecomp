// dcast<eTexCoordGen *>(const cBase *) @ 0x001cae98
// RTTI downcast: inline GetType type-init ladder + virtual GetType dispatch
// + parent-chain walk. 2-level cascade; mirrors the matched dcast<eBspTree *>
// shape. Constants from THIS function's 0x001cae98 disassembly:
//   inner D_000385DC : InitializeType(0x36CD74,0x36CD7C, 1, 0, 0, 0,0,0)
//   outer D_00041040 : InitializeType(0,0, 0x47, D_000385DC,
//                                      &eTexCoordGen::New, 0,0,0)
//   eTexCoordGen::New @ 0x1E4318. Shared eAll base strings 0x36CD74/0x36CD7C.

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

class eTexCoordGen {
public:
    static cBase *New(cMemPool *, cBase *);
};

template <class T> T dcast(const cBase *);

extern cType *D_000385DC;
extern cType *D_00041040;

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
        if (D_00041040 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                   (const char *)0x36CD7C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_00041040 = cType::InitializeType(0, 0, 0x47, D_000385DC,
                                               &eTexCoordGen::New, 0, 0, 0);
        }
        void *classDesc = *(void **)((char *)obj + 4);
        cType *target = D_00041040;
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

template eTexCoordGen *dcast<eTexCoordGen *>(const cBase *);
