// ODR-WARNING: split TU local redeclaration for one method. Do not add this
// method to the shared header; header churn can perturb matched siblings.

class gcExpression;

void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);
void cStrCopy(char *, const char *);

struct GetTextRec {
    short offset;
    short pad;
    void (*fn)(void *, char *);
};

struct GetTextSlot {
    short offset;
    short pad;
    void (*fn)(void *, char *);
};

struct TextNameRec {
    short offset;
    short pad;
    void *(*fn)(void *);
};

class gcDesiredEnumerationEntryHelper {
public:
    void GetText(char *, bool) const;
};

class gcDoObjectForEachRelationship {
public:
    void GetText(char *) const;
};

void gcDoObjectForEachRelationship::GetText(char *buf) const {
    register const gcDoObjectForEachRelationship *self __asm__("$16") = this;
    register char *out __asm__("$17") = buf;
    char text[256];
    char branchText[256];

    int value = *(int *)((const char *)self + 0x0C);
    text[0] = '\0';
    int owned = 0;
    int tagged = value & 1;
    if (tagged != 0) {
        owned = 1;
    }
    int valid;
    if (owned != 0) {
        valid = 0;
    } else {
        valid = value != 0;
        valid &= 0xFF;
        valid = valid != 0;
    }

    if (valid != 0) {
        int owned2 = 0;
        if (tagged != 0) {
            owned2 = 1;
        }
        if (owned2 != 0) {
            value = 0;
        } else {
            __asm__ volatile("" ::: "memory");
        }

        register const char *fmt __asm__("$20") = (const char *)0x36DCE4;
        int check = value;
        TextNameRec *nameRec = (TextNameRec *)(*(char **)(check + 4) + 0x80);
        void *nameObj = nameRec->fn((char *)check + nameRec->offset);
        cStrAppend(text, fmt, *(const char **)((const char *)nameObj + 0x0C));

        GetTextRec *rec = (GetTextRec *)(*(char **)(check + 4) + 0x78);
        rec->fn((char *)check + rec->offset, text);
        cStrAppend(text, (const char *)0x36DCEC);

        branchText[0] = '\0';
    } else {
        cStrCopy(text, (const char *)0x36DACC);
        branchText[0] = '\0';
    }

    char *branchStart = branchText;
    *branchStart = '\0';
    bool helperFlag = true;
    ((const gcDesiredEnumerationEntryHelper *)((const char *)self + 0x18))
        ->GetText(branchText, helperFlag);

    int branchOwned = 0;
    int branch = *(int *)((const char *)self + 0x24);
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
        cStrAppend(branchText, (const char *)0x36DCBC);

        branch = *(int *)((const char *)self + 0x24);
        int owned3 = 0;
        if (branch & 1) {
            owned3 = 1;
        }
        if (owned3 != 0) {
            branch = 0;
        } else {
            __asm__ volatile("" ::: "memory");
        }

        int branchCheck = branch;
        if (branchCheck != 0) {
            GetTextSlot *slot =
                (GetTextSlot *)(*(char **)(branchCheck + 4) + 0xD0);
            slot->fn((char *)branch + slot->offset, branchText);
        } else {
            cStrCat(branchText, (const char *)0x36DB24);
        }
    }

    cStrAppend(out, (const char *)0x36ECD4, text, branchText);
}
