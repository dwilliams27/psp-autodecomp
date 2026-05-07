// ODR-WARNING: split TU local redeclaration for one method. Do not add this
// method to the shared header; header churn can perturb matched siblings.

class gcDoEntitySendMessage {
public:
    void GetText(char *) const;
};

class gcDesiredEnumerationEntryHelper {
public:
    void GetText(char *, bool) const;
};

class gcDesiredEventParams {
public:
    void GetText(char *, bool) const;
};

struct GetTextSlot {
    short offset;
    short pad;
    void (*fn)(void *, char *);
};

void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);
extern "C" void __0fUgcDesiredEventParamsHGetTextPcbK(void *, char *, int);

void gcDoEntitySendMessage::GetText(char *buf) const {
    register const gcDoEntitySendMessage *self __asm__("$16") = this;
    register char *out __asm__("$17") = buf;
    char text[256];

    char *typeInfo0 = *(char **)((const char *)self + 0x10);
    GetTextSlot *slot0 = (GetTextSlot *)(typeInfo0 + 0x78);
    char *sub0 = (char *)self + 0x0C;
    slot0->fn(sub0 + slot0->offset, out);

    int mode = *(int *)((const char *)self + 0x60);
    register int sendFlag __asm__("$s3") = 0;
    register int two __asm__("$s2") = 2;
    if ((mode == 1) || (mode == two)) {
        sendFlag = 1;
    }

    const char *typeText;
    int kind = *(int *)((const char *)self + 0x5C);
    sendFlag &= 0xFF;
    switch (kind) {
    case 0:
        typeText = (const char *)0x36E924;
        break;
    case 1:
        typeText = (const char *)0x36E930;
        break;
    case 2:
        typeText = (const char *)0x36E93C;
        break;
    case 3:
        typeText = (const char *)0x36E94C;
        break;
    default:
        typeText = (const char *)0x36DAF0;
        break;
    }

    text[0] = '\0';
    char *textStart = text;
    *textStart = '\0';
    bool helperFlag = false;
    ((const gcDesiredEnumerationEntryHelper *)((const char *)self + 0x40))
        ->GetText(text, helperFlag);

    int branchOwned = 0;
    int branch = *(int *)((const char *)self + 0x4C);
    int branchTagged = branch & 1;
    if (branchTagged != 0) {
        branchOwned = 1;
    }
    int branchValid;
    if (branchOwned != 0) {
        branchValid = 0;
    } else {
        branchValid = branch != 0;
        branchValid &= 0xFF;
        branchValid = branchValid != 0;
    }

    if (branchValid != 0) {
        cStrAppend(text, (const char *)0x36DCBC);

        branch = *(int *)((const char *)self + 0x4C);
        int owned2 = 0;
        if (branch & 1) {
            owned2 = 1;
        }
        if (owned2 != 0) {
            branch = 0;
        } else {
            __asm__ volatile("" ::: "memory");
        }

        int check = branch;
        if (check != 0) {
            GetTextSlot *slot = (GetTextSlot *)(*(char **)(check + 4) + 0xD0);
            slot->fn((char *)branch + slot->offset, text);
        } else {
            cStrCat(text, (const char *)0x36DB24);
        }
    }

    const char *fmt = (const char *)0x36E95C;
    __asm__ volatile("" : "+r"(fmt));
    int modeAgain = *(int *)((const char *)self + 0x60);
    const char *sendText;
    if (sendFlag != 0) {
        sendText = (const char *)0x36E970;
    } else {
        sendText = (const char *)0x36E978;
    }

    const char *modeText;
    if (modeAgain == two) {
        modeText = (const char *)0x36E740;
    } else {
        modeText = (const char *)0x36D944;
    }

    cStrAppend(out, fmt, sendText, typeText, modeText, text);

    int paramsOwned = 0;
    int params = *(int *)((const char *)self + 0x58);
    int paramsTagged = params & 1;
    if (paramsTagged != 0) {
        paramsOwned = 1;
    }
    int paramsValid;
    if (paramsOwned != 0) {
        paramsValid = 0;
    } else {
        paramsValid = params != 0;
        paramsValid &= 0xFF;
        paramsValid = paramsValid != 0;
    }

    if (paramsValid != 0) {
        int owned3 = 0;
        if (paramsTagged != 0) {
            owned3 = 1;
        }
        if (owned3 != 0) {
            params = 0;
        } else {
            __asm__ volatile("" ::: "memory");
        }

        __0fUgcDesiredEventParamsHGetTextPcbK((void *)params, out, 1);
    }

    const char *suffix;
    if (**(int **)((const char *)self + 0x50) != 0) {
        suffix = (const char *)0x36DAD8;
    } else {
        suffix = (const char *)0x36E440;
    }
    cStrCat(out, suffix);
}
