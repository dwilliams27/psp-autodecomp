// gcDesiredObjectT<gcDesiredSkin, gcDesiredSkinHelper, eSkin> - gcAll_psp.obj
//
// Functions matched here:
//   gcDesiredObjectT<...>::GetDesiredType(void) const   @ 0x0027b13c (296B)
//   gcDesiredObjectT<...>::GetText(char *) const        @ 0x0027b264 (264B)

class cBase;
class cMemPool;
class cType;
class gcDesiredSkin;
class gcDesiredSkinHelper;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class eSkin {
public:
    static cBase *New(cMemPool *, cBase *);
};

class gcDesiredObjectHelper {
public:
    enum gcPrimary {};
    static const char *GetPrimaryText(gcPrimary);
};

extern "C" void cStrCat(char *, const char *);

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_000469F8;

template <class T1, class T2, class T3>
class gcDesiredObjectT {
public:
    void GetText(char *) const;
    const cType *GetDesiredType(void) const;
};

// ── gcDesiredObjectT<...>::GetDesiredType(void) const @ 0x0027b13c ──
template <class T1, class T2, class T3>
const cType *gcDesiredObjectT<T1, T2, T3>::GetDesiredType(void) const {
    if (D_000469F8 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType(
                        (const char *)0x36D894, (const char *)0x36D89C,
                        1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(
                    0, 0, 2, D_000385DC, &cNamed::New, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(
                0, 0, 3, D_000385E0, 0, 0, 0, 0);
        }
        D_000469F8 = cType::InitializeType(
            0, 0, 0x45, D_000385E4, &eSkin::New,
            (const char *)0x36DE64, (const char *)0x36DE6C, 5);
    }
    return D_000469F8;
}

// ── gcDesiredObjectT<...>::GetText(char *) const @ 0x0027b264 ──
template <class T1, class T2, class T3>
void gcDesiredObjectT<T1, T2, T3>::GetText(char *buf) const {
    int v8 = *(int *)((const char *)this + 8);
    int flag1 = 0;
    if (v8 & 1) {
        flag1 = 1;
    }
    int hasReal;
    if (flag1 != 0) {
        hasReal = 0;
    } else {
        int raw = (v8 != 0);
        hasReal = ((unsigned char)raw) != 0;
    }
    if (hasReal == 0) {
        int t = *(int *)((const char *)this + 12);
        const char *text;
        if (t == 7) {
            int hid = *(int *)((const char *)this + 16);
            void *entry;
            if (hid == 0) {
                entry = 0;
            } else {
                void **table = (void **)0x38890;
                void *cand = table[(unsigned short)hid];
                entry = 0;
                if (cand != 0 && *(int *)((char *)cand + 0x30) == hid) {
                    entry = cand;
                }
            }
            if (entry != 0) {
                short v = *(short *)((char *)entry + 0x1C);
                int isZero = (v == 0);
                isZero &= 0xFF;
                if (isZero == 0) {
                    text = (const char *)((char *)entry + 8);
                } else {
                    text = (const char *)0x36DAB8;
                }
            } else if (hid != 0) {
                text = (const char *)0x36DAC4;
            } else {
                text = (const char *)0x36DACC;
            }
        } else {
            text = gcDesiredObjectHelper::GetPrimaryText(
                (gcDesiredObjectHelper::gcPrimary)t);
        }
        cStrCat(buf, text);
    }
}

template class gcDesiredObjectT<gcDesiredSkin, gcDesiredSkinHelper, eSkin>;
