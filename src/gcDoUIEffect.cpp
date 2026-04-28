// gcDoUIEffect — gcAll_psp.obj
//   0x0030a2cc  Write(cFile &) const                       (112B)
//   0x0030a73c  ~gcDoUIEffect(void)                        (124B)

class cBase;
class cFile;
class cMemPool;

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

extern "C" {
    void *cMemPool_GetPoolFromPtr(const void *);
    void gcAction_Write(const void *, cFile &);
}

class gcAction {
public:
    cBase *mOwner;
    void *mVTable;
    unsigned int mNext;

    ~gcAction();
    void Write(cFile &) const;
};

class gcDesiredUIWidgetHelper {
public:
    void Write(class cWriteBlock &) const;
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class gcDoUIEffect : public gcAction {
public:
    void Write(cFile &) const;
    ~gcDoUIEffect();
    static void operator delete(void *p) {
        void *pool = cMemPool_GetPoolFromPtr(p);
        void *block = *(void **)((char *)pool + 0x24);
        DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
        short off = rec->offset;
        rec->fn((char *)block + off, p);
    }
};

extern char gcDoUIEffectvirtualtable[];

// 0x0030a2cc — Write(cFile &) const, 112B
void gcDoUIEffect::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    gcAction_Write(this, file);
    ((gcDesiredUIWidgetHelper *)((char *)this + 12))->Write(*(cWriteBlock *)&wb);
    wb.Write(((int *)this)[7]);
    wb.Write(((int *)this)[6]);
    wb.End();
}

// 0x0030a73c — ~gcDoUIEffect(void), 124B
gcDoUIEffect::~gcDoUIEffect() {
    ((void **)this)[1] = gcDoUIEffectvirtualtable;
}
