// gcBackgroundLoader::OnExit(void)  @ 0x000f7d34
// Split-TU: local declarations only; symbol __0fSgcBackgroundLoaderGOnExitv

typedef void (*SetFn4)(void *, int, void *, short);
typedef void (*SetFn3)(void *, short, void *);

struct SetEntry {
    short offset;
    short _pad;
    void *func;
};

class cBase {
public:
    void *_vt;       // 0x0
    char *mType;     // 0x4
};

class gcBackgroundLoader {
public:
    char pad_00[8];          // 0x000
    cBase *mObj8;            // 0x008
    char pad_0C[0x80C - 0xC];// 0x00C
    cBase *m80C;             // 0x80C
    cBase *m810;             // 0x810
    cBase *m814;             // 0x814
    cBase *m818;             // 0x818
    cBase *m81C;             // 0x81C

    void OnExit(void);
};

#define DESTROY(p)                                              \
    do {                                                        \
        char *type = (p)->mType;                                \
        SetEntry *entry = (SetEntry *)(type + 0x50);            \
        short adjust = entry->offset;                           \
        SetFn4 fn = (SetFn4)entry->func;                        \
        fn((char *)(p) + adjust, 3, (void *)fn, adjust);        \
    } while (0)

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0fSgcBackgroundLoaderGOnExitv, 0x10c\n");

void gcBackgroundLoader::OnExit(void) {
    {
        cBase *obj = mObj8;
        SetEntry *entry = (SetEntry *)((char *)obj + 8);
        short adjust = entry->offset;
        SetFn3 fn = (SetFn3)entry->func;
        fn((char *)this + adjust, adjust, (void *)fn);
    }

    if (m80C != 0) {
        DESTROY(m80C);
        m80C = 0;
    }
    if (m818 != 0) {
        DESTROY(m818);
        m818 = 0;
    }
    int i = 0;
    if (m81C != 0) {
        DESTROY(m81C);
        m81C = 0;
        i = 0;
    }

    gcBackgroundLoader *cur = this;
    do {
        if (cur->m810 != 0) {
            DESTROY(cur->m810);
            cur->m810 = 0;
        }
        i += 1;
        cur = (gcBackgroundLoader *)((char *)cur + 4);
    } while (i < 2);
}
