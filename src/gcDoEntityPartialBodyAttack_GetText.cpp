// ODR-WARNING: split TU local redeclaration for one method. Do not add this
// method to the shared header; header churn can perturb matched siblings.

class gcDoEntityPartialBodyAttack {
public:
    void GetText(char *) const;
};

class gcDesiredEnumerationEntryHelper {
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

void gcDoEntityPartialBodyAttack::GetText(char *buf) const {
    register const gcDoEntityPartialBodyAttack *self __asm__("$16") = this;
    register char *out __asm__("$17") = buf;
    char text[256];
    char targetText[256];

    char *typeInfo0 = *(char **)((const char *)self + 0x10);
    GetTextSlot *slot0 = (GetTextSlot *)(typeInfo0 + 0x78);
    char *sub0 = (char *)self + 0x0C;
    slot0->fn(sub0 + slot0->offset, out);

    text[0] = '\0';
    targetText[0] = '\0';

    if (*(int *)((const char *)self + 0x38) == 0) {
        char *textStart = text;
        *textStart = '\0';
        bool helperFlag = false;
        ((const gcDesiredEnumerationEntryHelper *)((const char *)self + 0x44))
            ->GetText(text, helperFlag);

        int branchOwned = 0;
        int branch = *(int *)((const char *)self + 0x50);
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

            branch = *(int *)((const char *)self + 0x50);
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
    } else {
        char *typeInfo1 = *(char **)((const char *)self + 0x58);
        GetTextSlot *slot1 = (GetTextSlot *)(typeInfo1 + 0x40);
        char *sub1 = (char *)self + 0x54;
        slot1->fn(sub1 + slot1->offset, text);
    }

    int target = *(int *)((const char *)self + 0x68);
    int owned3 = 0;
    if (target & 1) {
        owned3 = 1;
    }
    if (owned3 != 0) {
        target = 0;
    } else {
        __asm__ volatile("" ::: "memory");
    }

    int targetCheck = target;
    if (targetCheck != 0) {
        GetTextSlot *slot2 = (GetTextSlot *)(*(char **)(targetCheck + 4) + 0xD0);
        slot2->fn((char *)target + slot2->offset, targetText);
    } else {
        cStrCat(targetText, (const char *)0x36DB24);
    }

    int mode = *(int *)((const char *)self + 0x6C);
    if (mode < 2) {
        if (mode < 0) {
            goto done;
        }
        if (mode > 0) {
            goto mode_one;
        }
        cStrAppend(out, (const char *)0x36E78C, text);
        goto done;
    } else {
        if (mode < 3) {
            goto mode_two;
        }
        if (mode >= 4) {
            goto done;
        }
        cStrAppend(out, (const char *)0x36E7AC, text);
        goto done;
    }

mode_one:
    cStrAppend(out, (const char *)0x36E7D0, text, targetText);
    goto done;

mode_two:
    cStrAppend(out, (const char *)0x36E800, targetText);

done:
    ;
}
