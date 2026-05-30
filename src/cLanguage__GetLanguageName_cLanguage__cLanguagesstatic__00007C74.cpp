// cLanguage::GetLanguageName(cLanguage::cLanguages) static
// Address: 0x00007c74  Size: 188B  Obj: cAll_psp.obj
// Symbol: __0fJcLanguagePGetLanguageName65JcLanguageY__5JcLanguageKcLanguagesT
//
// ODR-WARNING: cLanguage is redeclared locally (split-TU). Only the members
// this TU references are present; do not add these to a canonical header.
//
// 12-case dense switch on the language enum -> jump table (snc-loop-switch.md:
// 8+ cases => jump table). Each case returns a baked string-constant pointer;
// the default returns one too. compare_func masks the .rodata relocations, so
// distinct string literals reproduce the per-case lui/addiu(jr ra) tails.

class cLanguage {
public:
    enum cLanguages {
        kLanguage_Invalid = 0
    };
    static const char *GetLanguageName(cLanguages languages);
};

const char *cLanguage::GetLanguageName(cLanguages languages)
{
    switch (languages) {
    case 0:  return "L00";
    case 1:  return "L01";
    case 2:  return "L02";
    case 3:  return "L03";
    case 4:  return "L04";
    case 5:  return "L05";
    case 6:  return "L06";
    case 7:  return "L07";
    case 8:  return "L08";
    case 9:  return "L09a";
    case 10: return "L10a";
    case 11: return "L11a";
    default: return "Lxxa";
    }
}
