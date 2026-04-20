// === Function 1: gcUIDialog::gcMouseOver::gcMouseOver(void) ===
class gcUIDialog {
public:
    class gcMouseOver {
    public:
        int mA;
        int mB;
        int mC;
        float mD;
        float mE;
        gcMouseOver();
    };
};

gcUIDialog::gcMouseOver::gcMouseOver() {
    mA = 0;
    mB = 0;
    mC = 0;
    mD = 0.0f;
    mE = 0.0f;
}

// === Function 2: gcTableColumnGUID::Get(int, cGUID *) const ===
struct cGUID {
    int mA;
    int mB;
};

class gcTableColumnGUID {
public:
    char _pad0[8];
    cGUID *mArray;    // 0x08
    void Get(int, cGUID *) const;
};

void gcTableColumnGUID::Get(int index, cGUID *out) const {
    *out = mArray[index];
}

// === Function 3: gcDoEvaluation::SetBranch(int, gcExpression *) ===
class gcExpression;

class gcDoEvaluation {
public:
    char _pad0[0x10];
    gcExpression *mB0;   // 0x10
    char _pad1[4];
    gcExpression *mB1;   // 0x18
    void SetBranch(int, gcExpression *);
};

void gcDoEvaluation::SetBranch(int index, gcExpression *expr) {
    if (index == 0) {
        mB0 = expr;
    } else if (index == 1) {
        mB1 = expr;
    }
}

// === Function 4: gcDoSwitch::SetBranch(int, gcExpression *) ===
class gcDoSwitch {
public:
    char _pad0[0x10];
    gcExpression *mB0;   // 0x10
    char _pad1[4];
    gcExpression *mB1;   // 0x18
    void SetBranch(int, gcExpression *);
};

void gcDoSwitch::SetBranch(int index, gcExpression *expr) {
    if (index == 0) {
        mB0 = expr;
    } else if (index == 1) {
        mB1 = expr;
    }
}

// === Function 5: cLanguage::GetSupportedVoiceLanguage(cLanguage::cLanguages) static ===
class cLanguage {
public:
    enum cLanguages {
        kLangUnknown = 0
    };
    struct LangConfig {
        char _pad[0x214];
        int mVoiceLang[16];
    };
    static LangConfig *sConfig;
    static int GetSupportedVoiceLanguage(cLanguages lang);
};

int cLanguage::GetSupportedVoiceLanguage(cLanguage::cLanguages lang) {
    int result = 0;
    if (cLanguage::sConfig != 0) {
        result = cLanguage::sConfig->mVoiceLang[lang];
    }
    return result;
}
