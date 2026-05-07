class cBase;
class cFile;
class cMemPool;

class cTimeValue {
public:
    int mTime;
};

extern "C" void free(void *);
extern "C" void *__vec_new(void *, int, int, void (*)(void *));
extern "C" void cFile_SetCurrentPos(void *file, unsigned int pos);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *rb, cFile &file,
                                             unsigned int id, bool validate);
extern "C" void __0oKcReadBlockdtv(void *rb, int flags);

struct cTypeMethod {
    short offset;
    short pad;
    void *fn;
};

class cType {
public:
    char _p0[0x28];
    cTypeMethod write_m;

    static cType *InitializeType(const char *, const char *, unsigned int, const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    cFile *file;
    unsigned int _pos;

    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class gcDesiredObject {
public:
    int _parent;
    cType *mType;
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcSubGeomController {
public:
    cBase *mParent;
    void *mVtable;
    int mField8;

    gcSubGeomController(cBase *);
    ~gcSubGeomController(void);
    const cType *GetType(void) const;
    int Read(cFile &, cMemPool *);
    void SetFade(float, cTimeValue);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        if (pool != 0) {
            void *block = *(void **)((char *)pool + 0x24);
            DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
            short off = rec->offset;
            rec->fn((char *)block + off, p);
        } else {
            free(p);
        }
    }
};

class gcValEntityTimeSince {
public:
    void Write(cFile &) const;
};

class cStr {
public:
    char _data[256];
};

class gcValNavMeshDirection {
public:
    void GetText(char *) const;
};

extern char gcSubGeomControllervirtualtable[];
extern cType *D_000385DC;
extern cType *D_0009F64C;

void gcLValue_Write(const gcValEntityTimeSince *, cFile &);
extern "C" cStr *__0oEcStrctPCce(cStr *, const char *, ...);
void cStrCat(char *, const char *);
void cStrAppend(char *, const char *, ...);

gcSubGeomController::gcSubGeomController(cBase *parent) {
    mParent = parent;
    mVtable = gcSubGeomControllervirtualtable;
    mField8 = 0;
    float zero = 0.0f;
    *(int *)((char *)this + 0x0C) = 0;
    float one = 1.0f;
    *(float *)((char *)this + 0x18) = zero;
    *(float *)((char *)this + 0x10) = one;
    *(float *)((char *)this + 0x14) = one;
    __vec_new((char *)this + 0x1C, 1, 6, (void (*)(void *))0x24400C);
}

gcSubGeomController::~gcSubGeomController(void) {
    mField8 = 0;
    mVtable = gcSubGeomControllervirtualtable;
}

const cType *gcSubGeomController::GetType(void) const {
    if (D_0009F64C == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType((const char *)0x36D894, (const char *)0x36D89C,
                                               1, 0, 0, 0, 0, 0);
        }
        D_0009F64C = cType::InitializeType(0, 0, 0x1D5, D_000385DC, 0, 0, 0, 0);
    }
    return D_0009F64C;
}

// 0x00147c74 — Read
int gcSubGeomController::Read(cFile &file, cMemPool *) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 1, true);
    if (rb[3] != 1) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }
    __0oKcReadBlockdtv(rb, 2);
    return result;
}

void gcValEntityTimeSince::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcLValue_Write(this, file);

    const cTypeMethod *lhs =
        (const cTypeMethod *)((char *)((const gcDesiredObject *)((const char *)this + 8))->mType + 40);
    char *lhsBase = (char *)this + 8;
    typedef void (*WriteFn)(void *, cFile *);
    ((WriteFn)lhs->fn)(lhsBase + lhs->offset, wb.file);

    const cTypeMethod *rhs =
        (const cTypeMethod *)((char *)((const gcDesiredObject *)((const char *)this + 0x34))->mType + 40);
    char *rhsBase = (char *)this + 0x34;
    ((WriteFn)rhs->fn)(rhsBase + rhs->offset, wb.file);

    wb.End();
}

// 0x00148090 -- gcSubGeomController::SetFade(float, cTimeValue)
void gcSubGeomController::SetFade(float fade, cTimeValue time) {
    if (time.mTime <= 0) {
        goto snap;
    }

    register float ftime __asm__("$f13");
    __asm__ volatile("mtc1 %1, %0" : "=f"(ftime) : "r"(time.mTime));
    *(float *)((char *)this + 0x14) = fade;
    int rate;
    __asm__ volatile("lui $5, 0x4");
    __asm__ volatile("cvt.s.w %0, %0" : "+f"(ftime));
    __asm__ volatile("lw %0, 3708($5)" : "=r"(rate) :: "$5");
    register float frate __asm__("$f14");
    __asm__ volatile("mtc1 %1, %0" : "=f"(frate) : "r"(rate));
    __asm__ volatile("cvt.s.w %0, %0" : "+f"(frate));
    register float t __asm__("$f15");
    __asm__ volatile("lui $5, 0x37\n\tlwc1 %0, -14336($5)" : "=f"(t) :: "$5");
    __asm__ volatile("mul.s %0, %2, %0\n\tmul.s %1, %2, %1\n\tdiv.s %0, %0, %1"
                     : "+f"(ftime), "+f"(frate)
                     : "f"(t));
    *(float *)((char *)this + 0x18) =
        (fade - *(float *)((char *)this + 0x10)) / ftime;
    return;

snap:
    *(float *)((char *)this + 0x10) = fade;
    *(float *)((char *)this + 0x14) = fade;
    *(float *)((char *)this + 0x18) = 0.0f;
}

// 0x00352278 -- gcValNavMeshDirection::GetText(char *) const
void gcValNavMeshDirection::GetText(char *buf) const {
    register const gcValNavMeshDirection *self __asm__("$17") = this;
    register char *out __asm__("$16") = buf;
    cStr tmp0;
    cStr tmp1;
    cStr tmp2;

    cTypeMethod *slot = (cTypeMethod *)(*(char **)((const char *)self + 0x0C) + 0x78);
    char *desired = (char *)self + 8;
    ((void (*)(void *, char *, char *))slot->fn)(desired + slot->offset, out,
                                                 desired);
    cStrCat(out, (const char *)0x36F5A8);

    int flags = *(int *)((const char *)self + 0x3C);
    if ((flags & 8) > 0) {
        cStrCat(out, (const char *)0x36F5B8);
    } else {
        register int val __asm__("$18") = *(int *)((const char *)self + 0x34);
        int tagged = 0;
        int tag = val & 1;
        if (tag != 0) {
            tagged = 1;
        }

        register int useFallback __asm__("$18");
        if (tagged != 0) {
            useFallback = 1;
        } else {
            int zero = val == 0;
            zero &= 0xFF;
            useFallback = zero != 0;
        }

        if (useFallback != 0) {
            cStrCat(out, (const char *)0x36F5D4);
        } else {
            cStrCat(out, (const char *)0x36E300);

            val = *(int *)((const char *)self + 0x34);
            tagged = 0;
            tag = val & 1;
            if (tag != 0) {
                tagged = 1;
            }
            if (tagged != 0) {
                val = 0;
            } else {
                __asm__ volatile("" ::: "memory");
            }

            char *type = *(char **)(val + 4);
            cTypeMethod *nameSlot = (cTypeMethod *)(type + 0x80);
            register const char *nameFmt __asm__("$19") = (const char *)0x36DCE4;
            void *nameObj =
                ((void *(*)(void *))nameSlot->fn)((char *)val + nameSlot->offset);
            cStrAppend(out, nameFmt, *(const char **)((char *)nameObj + 0x0C));

            cTypeMethod *textSlot = (cTypeMethod *)(*(char **)(val + 4) + 0x78);
            ((void (*)(void *, char *))textSlot->fn)(
                (char *)val + textSlot->offset, out);
            cStrAppend(out, (const char *)0x36DCEC);
            cStrCat(out, (const char *)0x36E2E8);

            int hasSuffix = *(short *)((const char *)self + 0x54) == 0;
            hasSuffix &= 0xFF;
            if (hasSuffix == 0) {
                cStrAppend(out, (const char *)0x36F5E0, (const char *)self + 0x40);
            }
        }
    }

    flags = *(int *)((const char *)self + 0x3C);
    int bit = flags & 1;
    if (bit > 0) {
        cStrCat(out, (const char *)__0oEcStrctPCce(
                         &tmp0, (const char *)0x36F5F0,
                         *(int *)((const char *)self + 0x58) + 1));
        flags = *(int *)((const char *)self + 0x3C);
    }

    bit = flags & 0x10;
    if (bit > 0) {
        cStrCat(out, (const char *)__0oEcStrctPCce(
                         &tmp1, (const char *)0x36F60C,
                         *(int *)((const char *)self + 0x5C) + 1));
        flags = *(int *)((const char *)self + 0x3C);
    }

    bit = flags & 0x20;
    if (bit > 0) {
        cStrCat(out, (const char *)__0oEcStrctPCce(
                         &tmp2, (const char *)0x36F62C,
                         *(int *)((const char *)self + 0x60) + 1));
        flags = *(int *)((const char *)self + 0x3C);
    }

    if ((flags & 4) > 0) {
        cStrCat(out, (const char *)0x36F64C);
    }
}
