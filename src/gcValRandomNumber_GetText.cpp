// ODR-WARNING: this TU intentionally redeclares gcValRandomNumber with only
// the layout and method needed for GetText, avoiding unrelated gcValRandomNumber
// methods perturbing SNC register allocation for this text-builder.

struct GetTextSlot {
    short offset;
    short pad;
    void (*fn)(void *, char *);
};

class gcValRandomNumber {
public:
    int pad0;
    int pad4;
    int mField8;
    int mDesiredC;
    int mDesired10;
    unsigned char mField14;

    void GetText(char *) const;
};

void cStrCat(char *, const char *);

#pragma control sched=2

void gcValRandomNumber::GetText(char *buf) const {
    char *out = buf;
    const gcValRandomNumber *self = this;
    cStrCat(out, (const char *)0x36F6F4);

    int kind = *(int *)((const char *)self + 8);
    if (kind < 2) {
        if (kind > 0) {
            cStrCat(out, (const char *)0x36F704);
            goto first_value;
        }
    } else if (kind < 3) {
        cStrCat(out, (const char *)0x36F70C);
    }

first_value:
    int val = *(int *)((const char *)self + 0xC);
    int tag = val & 1;
    int flag = 0;
    if (tag != 0) {
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
        slot->fn((char *)val + slot->offset, out);
    } else {
        cStrCat(out, (const char *)0x36DB24);
    }

    cStrCat(out, (const char *)0x36DAD8);

    val = *(int *)((const char *)self + 0x10);
    tag = val & 1;
    flag = 0;
    if (tag != 0) {
        flag = 1;
    }
    if (flag != 0) {
        val = 0;
    } else {
        __asm__ volatile("" ::: "memory");
    }

    check = val;
    if (check != 0) {
        char *typeInfo = *(char **)(check + 4);
        GetTextSlot *slot = (GetTextSlot *)(typeInfo + 0xD0);
        slot->fn((char *)val + slot->offset, out);
    } else {
        cStrCat(out, (const char *)0x36DB24);
    }

    cStrCat(out, (const char *)0x36DCEC);
}
