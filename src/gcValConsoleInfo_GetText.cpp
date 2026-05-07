// ODR-WARNING: split translation unit with a local class redeclaration.
// Keep this declaration limited to methods emitted in this file.

class gcValConsoleInfo {
public:
    void GetText(char *) const;
};

struct cTypeMethod {
    short offset;
    short pad;
    void *fn;
};

void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);

void gcValConsoleInfo::GetText(char *buf) const {
    cStrAppend(buf, (const char *)0x36F1AC, (const char *)0x36DAF0);

    int validKind = 0;
    int kind = *(int *)((const char *)this + 8);
    if ((kind == 0x15) || (kind == 0x16) || (kind == 0x17)) {
        validKind = 1;
    }
    validKind &= 0xFF;

    if (validKind != 0) {
        if (*(int *)((const char *)this + 0xC) == 0) {
            int val = *(int *)((const char *)this + 0x10);
            int tagged = 0;
            if (val & 1) {
                tagged = 1;
            }
            if (tagged != 0) {
                val = 0;
            } else {
                __asm__ volatile("" ::: "memory");
            }

            int check = val;
            if (check != 0) {
                char *type = *(char **)(check + 4);
                cTypeMethod *slot = (cTypeMethod *)(type + 0xD0);
                ((void (*)(void *, char *))slot->fn)(
                    (char *)val + slot->offset, buf);
            } else {
                cStrCat(buf, (const char *)0x36DB24);
            }
        } else {
            int val = *(int *)((const char *)this + 0x14);
            int tagged = 0;
            int tag = val & 1;
            if (tag != 0) {
                tagged = 1;
            }

            int useFallback;
            if (tagged == 0) {
                goto text_not_tagged;
            }
            useFallback = 1;
            goto text_fallback_done;
        text_not_tagged:
            int zero = val == 0;
            zero &= 0xFF;
            useFallback = zero != 0;
        text_fallback_done:

            if (useFallback != 0) {
                cStrAppend(buf, (const char *)0x36DACC);
            } else {
                char local[256];
                local[0] = '\0';

                int taggedAgain = 0;
                if (tag != 0) {
                    taggedAgain = 1;
                }

                char *type;
                int base = val;
                if (taggedAgain != 0) {
                    base = 0;
                    type = *(char **)(base + 4);
                } else {
                    type = *(char **)(base + 4);
                }

                cTypeMethod *slot = (cTypeMethod *)(type + 0x40);
                ((void (*)(void *, char *))slot->fn)(
                    (char *)base + slot->offset, local);
                cStrAppend(buf, local);
            }
        }
    }

    cStrAppend(buf, (const char *)0x36DCEC);
}
