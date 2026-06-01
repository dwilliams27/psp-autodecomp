// cLanguage::GetLanguage(const char *, cLanguage::cLanguages *) static
// Address: 0x00007f30, Size: 152B
// Obj: cAll_psp.obj

class cLanguage {
public:
    enum cLanguages {};
    static const char *GetLanguageName(cLanguages);
    static bool GetLanguage(const char *name, cLanguages *out);
};

extern int cStrCompare(const char *, const char *, bool);

bool cLanguage::GetLanguage(const char *name, cLanguages *out) {
    cLanguages i = (cLanguages)0;
    do {
        if (cStrCompare(name, GetLanguageName(i), true) == 0) {
            *out = i;
            return true;
        }
        i = (cLanguages)((int)i + 1);
    } while ((int)i < 12);
    return false;
}
