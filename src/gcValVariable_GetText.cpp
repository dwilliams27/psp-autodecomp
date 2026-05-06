// ODR-WARNING: split-TU method addition for gcValVariable. Keep this local
// redeclaration minimal so existing matched gcValVariable siblings do not drift.

class gcValVariable {
public:
    void GetText(char *) const;
};

extern void *D_00038890[];
void cStrCat(char *, const char *);

struct HandleObject {
    char pad0[8];
    char name[20];
    short nameLen;
    char pad1[18];
    int handle;
};

// gcValVariable::GetText(char *) const  @ 0x0036ae00, 172B
void gcValVariable::GetText(char *buf) const {
    int h = *(const int *)((const char *)this + 8);
    HandleObject *obj;
    if (h == 0) {
        obj = 0;
    } else {
        HandleObject *cand = (HandleObject *)D_00038890[h & 0xFFFF];
        obj = 0;
        if (cand != 0) {
            if (cand->handle == h) {
                obj = cand;
            }
        }
    }

    const char *text;
    if (obj != 0) {
        unsigned int empty = (obj->nameLen == 0);
        empty &= 0xFF;
        if (empty) {
            text = (const char *)0x36DAB8;
            goto object_done;
        }
        text = (const char *)obj + 8;
object_done:
        __asm__ volatile("" : : "r"(text));
        goto append;
    } else if (h != 0) {
        text = (const char *)0x36DAC4;
    } else {
        text = (const char *)0x36DACC;
    }
append:
    cStrCat(buf, text);
}
