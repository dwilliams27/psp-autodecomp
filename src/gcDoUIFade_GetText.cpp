// ODR-WARNING: this TU intentionally redeclares gcDoUIFade with a minimal
// member list because include/gcDoUIFade.h does not yet declare GetText.
// Keep this split from src/gcDoUIFade.cpp to avoid perturbing matched methods.

class gcDoUIFade {
public:
    void GetText(char *) const;
};

struct GetTextSlot {
    short offset;
    short _pad;
    void (*fn)(void *, char *);
};

struct HandleTextEntry {
    char _pad[0x1C];
    short count;
    short _pad2;
};

struct HandleLookupEntry {
    char _pad[0x30];
    int handle;
};

void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);

void gcDoUIFade::GetText(char *buf) const {
    register const gcDoUIFade *self __asm__("$16") = this;
    register char *out __asm__("$17") = buf;

    int modeFlag = *(int *)((const char *)self + 0x10);
    modeFlag &= 2;
    modeFlag = 0 < (unsigned int)modeFlag;
    modeFlag &= 0xFF;
    register const char *fmt __asm__("$4") = (const char *)0x36F020;
    int handle = *(int *)((const char *)self + 0x4C);
    register const char *mode __asm__("$6");
    if (modeFlag != 0) {
        mode = (const char *)0x36F030;
    } else {
        mode = (const char *)0x36F038;
    }

    register const char *sep __asm__("$7") = (const char *)0x36DAF0;
    register char *entry __asm__("$8");
    if (handle != 0) {
        goto have_handle;
    }
    entry = 0;
    goto lookup_done;

have_handle:
    {
        int index = handle & 0xFFFF;
        char **slot = (char **)((char *)0x38890 + (index << 2));
        char *candidate = *slot;
        entry = 0;
        if (candidate != 0 && ((HandleLookupEntry *)candidate)->handle == handle) {
            entry = candidate;
        }
    }

lookup_done:
    if (entry == 0) {
        goto no_entry;
    }
    {
        int empty = ((HandleTextEntry *)entry)->count == 0;
        entry += 8;
        if ((empty & 0xFF) != 0) {
            entry = (char *)0x36DAB8;
            goto have_name;
        }
        {
            register const char *fmtArg __asm__("$5") = fmt;
            __asm__ volatile("" : "+r"(fmtArg));
        }
        goto have_name;
    }

no_entry:
    if (handle != 0) {
        entry = (char *)0x36DAC4;
    } else {
        entry = (char *)0x36DACC;
    }

have_name:
    cStrAppend(out, fmt, mode, sep, entry);

    int child = *(int *)((const char *)self + 0x50);
    int owned = 0;
    if (child & 1) {
        owned = 1;
    }
    if (owned != 0) {
        child = 0;
    } else {
        __asm__ volatile("" ::: "memory");
    }

    if (child != 0) {
        cStrCat(out, (const char *)0x36EAF0);

        child = *(int *)((const char *)self + 0x50);
        int owned2 = 0;
        if (child & 1) {
            owned2 = 1;
        }
        if (owned2 != 0) {
            child = 0;
        } else {
            __asm__ volatile("" ::: "memory");
        }

        int check = child;
        if (check != 0) {
            char *typeInfo = *(char **)(check + 4);
            GetTextSlot *slot = (GetTextSlot *)(typeInfo + 0xD0);
            slot->fn((char *)child + slot->offset, out);
        } else {
            cStrCat(out, (const char *)0x36DB24);
        }
        cStrCat(out, (const char *)0x36E48C);
    }

    int flags = *(int *)((const char *)self + 0x10);
    if ((((flags & 4) != 0) & 0xFF) != 0) {
        cStrCat(out, (const char *)0x36F03C);
    }

    flags = *(int *)((const char *)self + 0x10);
    if ((((flags & 8) != 0) & 0xFF) != 0) {
        cStrCat(out, (const char *)0x36F04C);
    }

    if ((((*(int *)((const char *)self + 0x60) == 0) & 0xFF) == 0)) {
        cStrAppend(out, (const char *)0x36E6B8);
    }

    const char *suffix;
    if (**(int **)((const char *)self + 0x78) != 0) {
        suffix = (const char *)0x36DAD8;
    } else {
        suffix = (const char *)0x36E440;
    }
    cStrAppend(out, suffix);
}
