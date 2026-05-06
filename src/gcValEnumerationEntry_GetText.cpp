// ODR-WARNING: split-TU local redeclarations for adding GetText without
// changing the matched gcValEnumerationEntry translation unit.

class gcDesiredEnumerationEntryHelper {
public:
    void GetText(char *, bool) const;
};

class gcValEnumerationEntry {
public:
    void GetText(char *) const;
};

void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);

struct GetTextSlot {
    short offset;
    short pad;
    void (*fn)(void *, char *);
};

// gcValEnumerationEntry::GetText(char *) const @ 0x00342470
void gcValEnumerationEntry::GetText(char *buf) const {
    const gcValEnumerationEntry *owner = this;
    bool recursive = false;
    ((const gcDesiredEnumerationEntryHelper *)((const char *)this + 0x10))
        ->GetText(buf, recursive);

    int val = *(int *)((const char *)this + 0x1C);
    int flag = 0;
    if (val & 1) {
        flag = 1;
    }
    if (flag != 0) {
        val = 0;
    } else {
        val = val != 0;
        val &= 0xFF;
        val = val != 0;
    }

    if (val != 0) {
        cStrAppend(buf, (const char *)0x36DCBC);

        int objVal = *(int *)((const char *)owner + 0x1C);
        __asm__ volatile("" : "+r"(owner));
        int objFlag = 0;
        if (objVal & 1) {
            objFlag = 1;
        }
        if (objFlag != 0) {
            objVal = 0;
        } else {
            __asm__ volatile("" ::: "memory");
        }

        int objCheck = objVal;
        if (objCheck != 0) {
            char *typeInfo = *(char **)((unsigned)objCheck + 4);
            GetTextSlot *slot = (GetTextSlot *)(typeInfo + 0xD0);
            short off = slot->offset;
            void (*fn)(void *, char *) = slot->fn;
            fn((char *)objCheck + off, buf);
        } else {
            cStrCat(buf, (const char *)0x36DB24);
        }
    }
}
