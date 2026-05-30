// cLanguage::GetLanguageShortName(cLanguage::cLanguages) static @ 0x00007D30
// Split-TU: class declared locally to emit exactly this one method symbol.
// 12 dense cases (0..11) + default -> jump table switch.

class cLanguage {
public:
    enum cLanguages {
        kLang0 = 0,
        kLang1 = 1,
        kLang2 = 2,
        kLang3 = 3,
        kLang4 = 4,
        kLang5 = 5,
        kLang6 = 6,
        kLang7 = 7,
        kLang8 = 8,
        kLang9 = 9,
        kLang10 = 10,
        kLang11 = 11,
    };

    static const char *GetLanguageShortName(cLanguages);
};

const char *cLanguage::GetLanguageShortName(cLanguages lang) {
    switch (lang) {
    case kLang0:  return "en";
    case kLang1:  return "fr";
    case kLang2:  return "de";
    case kLang3:  return "it";
    case kLang4:  return "es";
    case kLang5:  return "nl";
    case kLang6:  return "pt";
    case kLang7:  return "ru";
    case kLang8:  return "ko";
    case kLang9:  return "cn";
    case kLang10: return "tw";
    case kLang11: return "jp";
    default:      return "xx";
    }
}
