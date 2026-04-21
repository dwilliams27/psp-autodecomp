// cLanguage.cpp — decompiled from cAll_psp.obj

class cStr {
public:
    char _data[256];
};

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

    static int PlatformInitialize(void);
    static int GetSystemLanguage(void);
    static int Initialize(void);
    static void OnLanguageChanged(void);
    static int GetLanguageIndex(void *);
    static int GetLanguageId(int);
    static unsigned char IsTextLanguageSupported(cLanguages);
    static void SetLanguage(cLanguages);
    static int GetLanguage(const char *, cLanguages *);
};

extern "C" {
    void cGetLanguageAndButton(int *lang, int *btn);
    int cGetCommandLineValue(const char *name, cStr *out);
    int sceImposeSetLanguageMode(int language, int button_assign);
    int sceUtilityGetSystemParamInt(int id, int *value);
}

// ─── cLanguage::OnLanguageChanged(void) static @ 0x000084ac ───
void cLanguage::OnLanguageChanged(void) {
    int lang;
    int btn;
    cGetLanguageAndButton(&lang, &btn);
    sceImposeSetLanguageMode(lang, btn);
}

// ─── cLanguage::GetSystemLanguage(void) static @ 0x00007b74 ───
int cLanguage::GetSystemLanguage(void) {
    int jp = 8;
    int val;
    sceUtilityGetSystemParamInt(8, &val);
    switch (val) {
    case 0: return jp;
    case 1: goto fallback;
    case 2: return 2;
    case 3: return 1;
    case 4: return 3;
    case 5: return 5;
    case 6: return 4;
    case 8: return 11;
    case 9: return 10;
    }
fallback:
    return 0;
}

// ─── cLanguage::Initialize(void) static @ 0x00007aec ───
int cLanguage::Initialize(void) {
    if (cLanguage::PlatformInitialize() == 0) {
        return 0;
    }
    int lang = cLanguage::GetSystemLanguage();
    cStr buf;
    buf._data[0] = 0;
    if (cGetCommandLineValue("language", &buf) != 0) {
        cLanguage::GetLanguage(buf._data, (cLanguages *)&lang);
    }
    cLanguage::SetLanguage((cLanguages)lang);
    return 1;
}

// ─── cLanguage::GetLanguageId(int) static @ 0x00008414 ───
int cLanguage::GetLanguageId(int target) {
    int matched = 0;
    for (int i = 0; i < 12; i++) {
        if (cLanguage::IsTextLanguageSupported((cLanguages)i)) {
            if (matched == target) {
                float f = (float)(i + 1);
                return *(int *)&f;
            }
            matched++;
        }
    }
    return 0;
}

// ─── cLanguage::GetLanguageIndex(void *) static @ 0x00008380 ───
int cLanguage::GetLanguageIndex(void *arg) {
    float idf = *(float *)&arg;
    int target = (int)idf - 1;
    int count = 0;
    for (int i = 0; i < 12; i++) {
        if (cLanguage::IsTextLanguageSupported((cLanguages)i)) {
            if (i == target) {
                return count;
            }
            count++;
        }
    }
    return -1;
}
