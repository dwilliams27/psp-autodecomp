#include "cObject.h"

// ─── helper classes (defined locally — not part of the cObject header) ───

struct cGUID_local {
    int a;
    int b;
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

class cType {
public:
    char _pad[0x14];
    int mField14;             // +0x14
};

extern int gSomePlatformDefault;          // 0x37C06C

char *cStrFormat(char *, const char *, ...);

// ============================================================
// cObject::GetLocalizedFilename(const cType *, const cGUID &, cStr *) static
// @ 0x0000a238, 168B
// ============================================================
void cObject::GetLocalizedFilename(const ::cType *type, const cGUID &guid, ::cStr *out) {
    ::cStr *o = out;
    const cGUID_local &g = (const cGUID_local &)guid;
    char buf[256];
    buf[0] = 0;
    const char *fmt = (const char *)0x36CA78;
    int t14 = ((const cType *)type)->mField14;
    const char *prefix = (const char *)0x38780;

    const char *langShort = cLanguage::GetLanguageShortName(
        (cLanguage::cLanguages)gSomePlatformDefault);

    cStrFormat(buf, (const char *)0x36C89C, g.a, g.b);

    ((cStr *)o)->Set(fmt, prefix, t14, langShort, buf);
}
