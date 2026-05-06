#include "cObject.h"

// ─── helper classes (defined locally — not part of the cObject header) ───

class cFile {
public:
    char _pad[0x108];
    unsigned int GetCurrentPos(void) const;
    void SetCurrentPos(unsigned int);
};

class cType {
public:
    int _pad0;
    unsigned int mTypeId;     // +0x04, written by WriteHeader
    char _pad8[0x0C];         // +0x08
    int mField14;             // +0x14, read by GetLocalizedFilename

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

struct cGUID_local {
    int a;
    int b;
    static cGUID_local Generate(void);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(unsigned int);
    void Write(unsigned short);
    void Write(const cGUID_local &);
    void End(void);
};

class cName_local {
public:
    void Set(const char *, ...);
    void Write(cWriteBlock &) const;   // address 0x8c18
};

class cStr {
public:
    char _data[256];
    void Set(const char *, ...);
};

class cLanguage {
public:
    enum cLanguages { kLang0 = 0 };
    static const char *GetLanguageShortName(cLanguages);
};

extern int gSomePlatformDefault;          // 0x37C06C

struct cObjectDispatchRecord {
    short offset;
    short pad;
    void *fn;
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
// cObject::WriteHeader(cFile &)
// @ 0x0000a750, 216B
// ============================================================
int cObject::WriteHeader(cFile &file) {
    cWriteBlock wb(file, 2);
    int pos = (int)file.GetCurrentPos();
    wb.Write(pos);
    wb.Write((unsigned int)0x56565656);
    wb.Write(*(const cGUID_local *)((const char *)this + 0x20));
    ((const cName_local *)((const char *)this + 8))->Write(wb);

    cObjectDispatchRecord *r1 =
        (cObjectDispatchRecord *)(*(char **)((char *)this + 4) + 8);
    cType *t = ((cType *(*)(void *))r1->fn)((char *)this + r1->offset);
    wb.Write((unsigned int)t->mTypeId);

    cObjectDispatchRecord *r2 =
        (cObjectDispatchRecord *)(*(char **)((char *)this + 4) + 0x98);
    unsigned int v = ((unsigned int (*)(void *))r2->fn)((char *)this + r2->offset);
    wb.Write(v);

    wb.Write(*(unsigned int *)((char *)this + 0x2C));
    wb.End();
    return pos;
}

// ============================================================
// cObject::GetLocalizedFilename(const cType *, const cGUID &, cStr *) static
// @ 0x0000a238, 168B
// ============================================================
//
// FAILED: 64/168 byte diff (down from 73 via permuter).
// Structure is correct. The expected layout allocates:
//   s1=guid, s2=t14, s3=fmt(0x36CA78), s0=out, s4=prefix(0x38780), s5=langShort
// SNC keeps wanting to allocate guid → s0 (lowest free saved reg) regardless
// of source ordering, asm barriers, or register asm("$N") constraints. The
// permuter exhausted 4080 candidates without breaking past this allocation.
// Likely needs the same kind of compiler-level fix as ML2 (cReadBlock prologue).
void cObject::GetLocalizedFilename(const cType *type, const cGUID &guid, cStr *out) {
    const char *fmt = (const char *)0x36CA78;
    const cGUID *gptr = &guid;
    __asm__ volatile("" : "+r"(gptr));
    char buf[256];
    cStr *o = out;
    buf[0] = 0;
    const char *prefix = (const char *)0x38780;
    __asm__ volatile("" : "+r"(fmt));
    int t14 = ((const ::cType *)type)->mField14;
    __asm__ volatile("" : "+r"(o), "+r"(prefix));

    const char *langShort = cLanguage::GetLanguageShortName(
        (cLanguage::cLanguages)gSomePlatformDefault);

    cStrFormat(buf, (const char *)0x36C89C,
               ((const int *)gptr)[0], ((const int *)gptr)[1]);

    o->Set(fmt, prefix, t14, langShort, buf);
}

// ============================================================
// cObject::IsEditable(void) const
// @ 0x0000a50c, 128B
// ============================================================
namespace {
struct cObjectVTableEntry {
    short offset;          // +0 this-adjust
    short pad;             // +2
    int *(*fn)(void *);    // +4 function pointer (returns ptr to int)
};
}

int cObject::IsEditable(void) const {
    unsigned short flags = *(const unsigned short *)((const char *)this + 0x28);
    if (flags & 0x20) return 0;
    int ro = ((flags & 0x08) != 0) & 0xFF;
    if (ro) return 0;

    char *vtable = *(char **)((char *)this + 4);
    cObjectVTableEntry *entry = (cObjectVTableEntry *)(vtable + 8);
    short adj = entry->offset;
    int *(*fn)(void *) = entry->fn;
    int *result = fn((char *)this + adj);

    int v = 0;
    if (*result & 1) v = 1;
    if (v & 0xFF) return 1;
    return 0;
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
    unsigned int x = *(unsigned int *)((char *)this + 0x30);
    register int zero __asm__("$0");
    if (((zero != x) & 0xFF) != 0) {
        *(unsigned short *)((char *)this + 0x28) |=
            (unsigned short)(flags & 0xFE00);
    }
}
