// Isolated for permuter — gcValUserStatus::GetText @ 0x001302e8

class cBase;

class gcValue {
public:
    char _pad[8];
};

class gcValUserStatus : public gcValue {
public:
    int   mPad8;
    int   mPadC;
    void GetText(char *) const;
};

extern const char gcValUserStatus_fmt1[];
extern const char gcValUserStatus_fmt2[];
extern const char gcValUserStatus_str_open[];
extern const char gcValUserStatus_str_close[];

void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);

typedef void (*GetTextVFn)(void *, char *);
struct GetTextThunk {
    short offset;
    short _pad;
    GetTextVFn fn;
};

void gcValUserStatus::GetText(char *buf) const {
    cStrAppend(buf, gcValUserStatus_fmt1, gcValUserStatus_fmt2);
    int val = mPad8;
    int notSix = (val != 6);
    notSix &= 0xFF;
    if (notSix != 0) {
        int valC = mPadC;
        int flag = 0;
        if (valC & 1) {
            flag = 1;
        }
        if (flag != 0) {
            valC = 0;
        } else {
            __asm__ volatile("" ::: "memory");
        }
        int check = valC;
        if (check != 0) {
            char *classdesc = *(char **)(check + 4);
            GetTextThunk *thunk = (GetTextThunk *)(classdesc + 0xD0);
            ((void (*)(void *, char *))thunk->fn)(
                (char *)check + thunk->offset, buf);
        } else {
            cStrCat(buf, gcValUserStatus_str_open);
        }
    }
    cStrCat(buf, gcValUserStatus_str_close);
}
