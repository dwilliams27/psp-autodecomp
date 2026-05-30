// cLanguage::GetLanguageNameLocalized(cLanguage::cLanguages) static
// Address: 0x00007dec  Size: 188B  Obj: cAll_psp.obj
// Symbol: __0fJcLanguageYGetLanguageNameLocalized65JcLanguageY__5JcLanguageKcLanguagesT
//
// ODR-WARNING: cLanguage is redeclared locally (split-TU). Only the members
// this TU references are present; do not add these to a canonical header.
//
// Dense 12-case switch -> jump table (snc-loop-switch.md "8+ cases").
// Each case returns a distinct localized language-name string literal;
// out-of-range falls through to the default literal. Relocations are masked
// by compare_func, so the concrete string addresses are irrelevant — only the
// jump-table control flow + the lui/addiu return shape must match.

class cLanguage {
public:
    enum cLanguages {
        kLang0 = 0, kLang1, kLang2, kLang3, kLang4, kLang5,
        kLang6, kLang7, kLang8, kLang9, kLang10, kLang11
    };
    static const char *GetLanguageNameLocalized(cLanguages languages);
};

const char *cLanguage::GetLanguageNameLocalized(cLanguages languages)
{
    switch (languages) {
    case kLang0:  return "L0";
    case kLang1:  return "L1";
    case kLang2:  return "L2";
    case kLang3:  return "L3";
    case kLang4:  return "L4";
    case kLang5:  return "L5";
    case kLang6:  return "L6";
    case kLang7:  return "L7";
    case kLang8:  return "L8";
    case kLang9:  return "L9";
    case kLang10: return "L10";
    case kLang11: return "L11";
    default:      return "LD";
    }
}
