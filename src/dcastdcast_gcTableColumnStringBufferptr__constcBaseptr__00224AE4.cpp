// dcast<gcTableColumnStringBuffer *>(const cBase *) @ 0x00224ae4
// RTTI downcast (gcAll_psp.obj). 3-level lazy cType registration cascade
// then a dynamic-type chain walk. Mirrors the matched gcTableColumnStringTable
// 3-level shape. Constants read from this fn's own binary:
//   inner  D_000385DC : InitializeType(0x36D894, 0x36D898+4, 1,     0,    0,    0, 0, 0)
//   middle D_0009F478 : InitializeType(0, 0, 0x241, D_000385DC, 0,    0, 0, 0)
//   outer  D_0009F48C : InitializeType(0, 0, 0x246, D_0009F478, &New, 0, 0, 0)
// gcTableColumnStringBuffer::New @ 0x00272c2c. Outer trailing uint arg is 0.
// Outer slot D_0009F48C is unique to this function and in range.

class cBase;
class cMemPool;
class cType;

class cType {
public:
    char _pad[0x1C];
    const cType *mParent;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcTableColumnStringBuffer {
public:
    static cBase *New(cMemPool *, cBase *);
};

struct DispatchEntry {
    short offset;
    short pad;
    cType *(*fn)(void *, short, void *);
};

extern cType *D_000385DC;
extern cType *D_0009F478;
extern cType *D_0009F48C;

template <class T>
T dcast(const cBase *base)
{
    const void *result = 0;

    if (base != 0) {
        if (D_0009F48C == 0) {
            if (D_0009F478 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)(0x36D898 + 4),
                                                       1, 0, 0, 0, 0, 0);
                }
                D_0009F478 = cType::InitializeType(0, 0, 0x241, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_0009F48C = cType::InitializeType(0, 0, 0x246, D_0009F478,
                                               &gcTableColumnStringBuffer::New, 0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009F48C;
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

template gcTableColumnStringBuffer *dcast<gcTableColumnStringBuffer *>(const cBase *obj);
