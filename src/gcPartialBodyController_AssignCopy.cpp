// ODR-WARNING: local redeclaration for a split-TU method addition.
// Keep this class shape limited to methods referenced in this translation unit.

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

class gcPartialEntityController {
public:
    static cBase *New(cMemPool *, cBase *);
};

class gcPartialBodyController : public gcPartialEntityController {
public:
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
};

struct DispatchEntry {
    short offset;
    short _pad;
    cType *(*fn)(void *, short, void *);
};

extern cType *D_000385DC;
extern cType *D_0009F5E0;
extern cType *D_0009F5F4;

// 0x002a5b08 -- gcPartialBodyController::AssignCopy(const cBase *)
void gcPartialBodyController::AssignCopy(const cBase *base) {
    const gcPartialBodyController *other = 0;
    const gcPartialBodyController *copy;

    if (base != 0) {
        if (D_0009F5F4 == 0) {
            if (D_0009F5E0 == 0) {
                if (D_000385DC == 0) {
                    const char *name = (const char *)0x36D894;
                    const char *desc = (const char *)0x36D89C;
                    D_000385DC = cType::InitializeType(name, desc, 1,
                                                       0, 0, 0, 0, 0);
                }
                cBase *(*factory)(cMemPool *, cBase *) =
                    (cBase *(*)(cMemPool *, cBase *))0x2A0964;
                D_0009F5E0 = cType::InitializeType(0, 0, 0x105, D_000385DC,
                                                   factory, 0, 0, 0);
            }
            cBase *(*factory)(cMemPool *, cBase *) =
                (cBase *(*)(cMemPool *, cBase *))0x2A5D90;
            D_0009F5F4 = cType::InitializeType(0, 0, 0x107, D_0009F5E0,
                                               factory, 0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009F5F4;
        DispatchEntry *entry = (DispatchEntry *)((char *)classDesc + 8);
        short offset = entry->offset;
        cType *(*fn)(void *, short, void *) = entry->fn;
        cType *type = fn((char *)base + offset, offset, fn);
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
            other = (const gcPartialBodyController *)base;
        }
    }

    copy = other;

    *(int *)((char *)this + 0x08) = *(const int *)((const char *)copy + 0x08);
    *(int *)((char *)this + 0x0C) = *(const int *)((const char *)copy + 0x0C);
    *(unsigned char *)((char *)this + 0x10) =
        *(const unsigned char *)((const char *)copy + 0x10);
    *(unsigned char *)((char *)this + 0x11) =
        *(const unsigned char *)((const char *)copy + 0x11);

    const int *srcWord = (const int *)((const char *)copy + 0x14);
    const int *nextSrc;
    int *dstWord = (int *)((char *)this + 0x14);
    *dstWord = *srcWord;
    srcWord = (const int *)((const char *)copy + 0x18);
    dstWord = (int *)((char *)this + 0x18);
    nextSrc = (const int *)((const char *)copy + 0x1C);
    *dstWord = *srcWord;
    dstWord = (int *)((char *)this + 0x1C);
    srcWord = nextSrc;
    nextSrc = (const int *)((const char *)copy + 0x20);
    *dstWord = *srcWord;
    srcWord = nextSrc;
    dstWord = (int *)((char *)this + 0x20);
    nextSrc = (const int *)((const char *)copy + 0x24);
    *dstWord = *srcWord;
    srcWord = nextSrc;
    dstWord = (int *)((char *)this + 0x24);
    nextSrc = (const int *)((const char *)copy + 0x28);
    *dstWord = *srcWord;
    srcWord = nextSrc;
    int *dstLast = (int *)((char *)this + 0x28);
    int lastWord = *srcWord;
    *dstLast = lastWord;

    *(int *)((char *)this + 0x2C) = *(const int *)((const char *)copy + 0x2C);
    *(unsigned short *)((char *)this + 0x30) =
        *(const unsigned short *)((const char *)copy + 0x30);

    int i = 0;
    short *dst = (short *)((char *)this + 0x32);
    const short *src = (const short *)((const char *)copy + 0x32);
    do {
        short x = src[0];
        short y = src[1];
        short z = src[2];
        dst[0] = x;
        dst[1] = y;
        dst[2] = z;
        i += 1;
        dst = (short *)((char *)dst + 6);
        src = (const short *)((const char *)src + 6);
    } while (i <= 0);

    *(int *)((char *)this + 0x38) = *(const int *)((const char *)copy + 0x38);
    *(int *)((char *)this + 0x3C) = *(const int *)((const char *)copy + 0x3C);
    *(int *)((char *)this + 0x40) = *(const int *)((const char *)copy + 0x40);
    *(unsigned char *)((char *)this + 0x44) =
        *(const unsigned char *)((const char *)copy + 0x44);
    *(float *)((char *)this + 0x48) = *(const float *)((const char *)copy + 0x48);

    int j = 0;
    short *dst2 = (short *)((char *)this + 0x4C);
    const short *src2 = (const short *)((const char *)copy + 0x4C);
    do {
        short x = src2[0];
        short y = src2[1];
        short z = src2[2];
        dst2[0] = x;
        dst2[1] = y;
        dst2[2] = z;
        j += 1;
        dst2 = (short *)((char *)dst2 + 6);
        src2 = (const short *)((const char *)src2 + 6);
    } while (j <= 0);
}
