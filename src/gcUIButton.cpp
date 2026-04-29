// gcUIButton -- decompiled from gcAll_psp.obj.
// Functions:
//   0x0013984c  ~gcUIButton(void)
//   0x0028e558  New(cMemPool *, cBase *) static
//   0x00139654  Write(cFile &) const
//   0x00139a28  IsUpdateEmpty(bool, bool) const

class cBase;
class cFile;
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

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

class gcUIWidget {
public:
    int IsUpdateEmpty(bool, bool) const;
};

class gcUITextControl : public gcUIWidget {
public:
    gcUITextControl(cBase *);
    ~gcUITextControl();
    int IsUpdateEmpty(bool, bool) const;
    void Write(cFile &) const;
};

class gcUIButton : public gcUITextControl {
public:
    gcUIButton(cBase *);
    ~gcUIButton();
    void Write(cFile &) const;
    int IsUpdateEmpty(bool, bool) const;

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

// -- gcUIButton::Write(cFile &) const @ 0x00139654 --
void gcUIButton::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcUITextControl *)this)->Write(file);
    wb.Write(2);

    int i = 0;
    cHandle *handle = (cHandle *)((char *)this + 0x114);
    do {
        handle->Write(wb);
        i += 1;
        handle = (cHandle *)((char *)handle + 4);
    } while (i < 2);

    wb.End();
}

// -- gcUIButton::IsUpdateEmpty(bool, bool) const @ 0x00139a28 --
int gcUIButton::IsUpdateEmpty(bool a, bool b) const {
    if (a) {
        int flag = (*(unsigned int *)((const char *)this + 0x24) & 8) != 0;
        flag &= 0xFF;
        if (flag) {
            return 0;
        }
    }
    return ((const gcUIWidget *)this)->IsUpdateEmpty(a, b);
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
