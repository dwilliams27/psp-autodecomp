#include "cObject.h"

// ─── helper classes (defined locally — not part of the cObject header) ───

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned short);
    void End(void);
};

class cName_local {
public:
    void Set(const char *, ...);
};

struct cGUID_local {
    int a;
    int b;
    static cGUID_local Generate(void);
};

char *cStrFormat(char *, const char *, ...);

// Format string at 0x0036C89C in .rodata.
extern const char cObject_guid_fmt[];

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;

// ============================================================
// cObject::GetType(void) const
// @ 0x001c6f64, 220B
// ============================================================
const cType *cObject::GetType(void) const {
    if (D_000385E4 == 0) {
        if (D_000385E0 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36C830,
                                                   (const char *)0x36C838,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                               &cNamed::New, 0, 0, 0);
        }
        D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                           0, 0, 0, 0);
    }
    return D_000385E4;
}

// ============================================================
// cObject::SetDirty(void)
// @ 0x???, already matched (stub)
// ============================================================
void cObject::SetDirty(void) {
}

// ============================================================
// cObject::GetNameFromGUID(const cGUID &, const cType *, bool, bool, cName *) static
// @ 0x0000a4c0, 76B
// ============================================================
int cObject::GetNameFromGUID(const cGUID &guid, const cType *, bool, bool, cName *name) {
    char buf[256];
    buf[0] = 0;
    cStrFormat(buf,
               cObject_guid_fmt,
               ((const cGUID_local &)guid).a,
               ((const cGUID_local &)guid).b);
    ((cName_local *)name)->Set(buf);
    return 1;
}

// ============================================================
// cObject::operator=(const cObject &)
// @ 0x00009d70, 84B
// ============================================================
namespace {
struct cObject_NameData { int w[6]; };
}

cObject &cObject::operator=(const cObject &other) {
    *(cObject_NameData *)((char *)this + 8) =
        *(const cObject_NameData *)((const char *)&other + 8);
    *(unsigned short *)((char *)this + 0x28) =
        (unsigned short)(*(unsigned short *)((char *)this + 0x28) |
                         (*(const unsigned short *)((const char *)&other + 0x28) & 0x40));
    return *this;
}

// ============================================================
// cObject::Copy(cMemPool *, cBase *) const
// @ 0x00009f74, 84B
// ============================================================
void cObject::Copy(cMemPool *pool, cBase *) const {
    cFactory *factory = *(cFactory **)((char *)pool + 0x28);
    cGUID_local guid = cGUID_local::Generate();
    factory->CopyObject(this, (const cGUID &)guid);
}

// ============================================================
// cObject::Write(cFile &) const
// @ 0x00009b30, 92B
// ============================================================
void cObject::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const cNamed *)this)->Write(file);
    wb.Write((unsigned short)(*(const unsigned short *)((const char *)this + 0x28) & 0x60));
    wb.End();
}

// ============================================================
// cObject::VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int)
// @ 0x00009cb0, 120B
// ============================================================
void cObject::VisitReferences(
    unsigned int flags,
    cBase *base,
    void (*callback)(cBase *, unsigned int, void *),
    void *ctx,
    unsigned int)
{
    if (callback != 0) {
        callback(base, (unsigned int)this, ctx);
    }
    if (*(int *)((char *)this + 0x30) != 0) {
        *(unsigned short *)((char *)this + 0x28) |=
            (unsigned short)(flags & 0xFE00);
    }
}
