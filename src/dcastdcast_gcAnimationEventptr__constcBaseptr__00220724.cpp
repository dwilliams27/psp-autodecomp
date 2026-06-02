// dcast<gcAnimationEvent *>(const cBase *) @ 0x00220724
// RTTI downcast: inline GetType type-init ladder + virtual GetType dispatch
// + parent-chain walk. Mirrors the matched 280B inline-dcast shape
// (eBspTree::dcast @ 0x001ca708) with this function's own constants:
//   outer D_00099AB4, shared base D_000385DC, names 0x36D894/0x36D89C,
//   ordinal 0x179, &gcAnimationEvent::New @ 0x0023f108.

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

class gcAnimationEvent {
public:
    static cBase *New(cMemPool *, cBase *);
};

template <class T> T dcast(const cBase *);

extern cType *D_000385DC;
extern cType *D_00099AB4;

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
        if (D_00099AB4 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_00099AB4 = cType::InitializeType(0, 0, 0x179, D_000385DC,
                                               &gcAnimationEvent::New, 0, 0, 0);
        }
        void *classDesc = *(void **)((char *)obj + 4);
        cType *target = D_00099AB4;
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

template gcAnimationEvent *dcast<gcAnimationEvent *>(const cBase *);
