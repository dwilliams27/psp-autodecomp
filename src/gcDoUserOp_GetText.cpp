// ODR-WARNING: split-TU local redeclaration for adding GetText without
// changing the matched gcDoUserOp translation unit.

class gcDoUserOp {
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

// -- gcDoUserOp::GetText @ 0x00317380 --
void gcDoUserOp::GetText(char *buf) const {
    cStrAppend(buf, (const char *)0x36E00C, (const char *)0x36DAF0);

    int val = *(int *)((const char *)this + 0x10);
    int flag = 0;
    if (val & 1) {
        flag = 1;
    }
    if (flag != 0) {
        val = 0;
    } else {
        __asm__ volatile("" ::: "memory");
    }
    int check = val;
    if (check != 0) {
        char *typeInfo = *(char **)(check + 4);
        GetTextSlot *slot = (GetTextSlot *)(typeInfo + 0xD0);
        slot->fn((char *)val + slot->offset, buf);
    } else {
        cStrCat(buf, (const char *)0x36DB24);
    }

    cStrCat(buf, (const char *)0x36DCEC);
}
