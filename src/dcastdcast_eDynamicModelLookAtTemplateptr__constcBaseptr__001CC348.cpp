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

class eDynamicModelLookAtTemplate {
public:
    static cBase *New(cMemPool *, cBase *);
};

// cBase type-info layout reconstructed from disassembly.
struct cTypeNode {
    char pad0[0x1C];
    cTypeNode *parent;        // +0x1C
};

// Descriptor cursor located at (cBase->info + 8): offset@0, getType@4.
struct cTypeCursor {
    short offset;             // +0
    char pad2[2];
    cTypeNode *(*getType)(cBase *, short, void *); // +4
};

struct cBaseLayout {
    char pad0[4];
    char *info;               // +4
};

extern cType *D_000385DC;
extern cType *D_000469E4;

template <class T>
T dcast(const cBase *obj)
{
    T result = 0;
    if (obj != 0) {
        if (D_000469E4 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                   (const char *)0x36CD7C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_000469E4 = cType::InitializeType(0, 0, 0x48, D_000385DC,
                                               &eDynamicModelLookAtTemplate::New,
                                               0, 0, 0);
        }
        cTypeCursor *cur = (cTypeCursor *)(((cBaseLayout *)obj)->info + 8);
        cTypeNode *target = (cTypeNode *)D_000469E4;
        short off = cur->offset;
        cTypeNode *(*fn)(cBase *, short, void *) = cur->getType;
        cTypeNode *t = fn((cBase *)((char *)obj + off), off, (void *)fn);
        int match;
        if (target == 0) {
            match = 0;
        } else if (t != 0) {
            for (;;) {
                if (t == target) {
                    match = 1;
                    break;
                }
                t = t->parent;
                if (t == 0) {
                    goto nomatch;
                }
            }
        } else {
        nomatch:
            match = 0;
        }
        if (match) {
            result = (T)obj;
        }
    }
    return result;
}

template eDynamicModelLookAtTemplate *dcast<eDynamicModelLookAtTemplate *>(const cBase *obj);
