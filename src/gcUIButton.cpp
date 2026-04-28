// gcUIButton -- decompiled from gcAll_psp.obj.
// Functions:
//   0x0013984c  ~gcUIButton(void)
//   0x0028e558  New(cMemPool *, cBase *) static

class cBase;
class cMemPool;

inline void *operator new(unsigned int, void *p) {
    return p;
}

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class cMemPoolHelper {
public:
    static cMemPoolHelper *GetPoolFromPtr(const void *);
};

class gcUITextControl {
public:
    gcUITextControl(cBase *);
    ~gcUITextControl();
};

class gcUIButton : public gcUITextControl {
public:
    gcUIButton(cBase *);
    ~gcUIButton();

    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPoolHelper *pool = cMemPoolHelper::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

extern char gcUIButtonvirtualtable[];

gcUIButton::~gcUIButton() {
    *(void **)((char *)this + 4) = gcUIButtonvirtualtable;
}

// -- gcUIButton::New(cMemPool *, cBase *) static @ 0x0028e558 --
cBase *gcUIButton::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcUIButton *result = 0;
    gcUIButton *obj = (gcUIButton *)rec->fn(base, 0x11C, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcUIButton(parent);
        result = obj;
    }
    return (cBase *)result;
}
