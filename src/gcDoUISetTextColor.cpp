// gcDoUISetTextColor.cpp — decompiled from gcAll_psp.obj.
// Parent: gcAction.
//
// Functions:
//   0x00314fdc gcDoUISetTextColor::Write(cFile &) const   112B
//   0x003153f0 gcDoUISetTextColor::~gcDoUISetTextColor(void)  124B

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(unsigned int);
    void End(void);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcExpression {
public:
    void Write(cFile &) const;
};

class gcAction : public gcExpression {
public:
    char _pad[12];   // mOwner, mVTable, mNext at 0/4/8

    gcAction(cBase *);
    ~gcAction();
    void Write(cFile &) const;
};

class gcDesiredUIWidgetHelper {
public:
    char _pad[12];   // 3 ints
    void Write(cWriteBlock &) const;
};

class gcDoUISetTextColor : public gcAction {
public:
    gcDesiredUIWidgetHelper mWidget;  // 0x0C
    int  mTextColorInt;                // 0x18
    unsigned int mTextColorUint;       // 0x1C

    gcDoUISetTextColor(cBase *);
    ~gcDoUISetTextColor();
    void Write(cFile &) const;

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

extern char gcDoUISetTextColorvirtualtable[];

// ── gcDoUISetTextColor::Write @ 0x00314fdc ──
void gcDoUISetTextColor::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction::Write(file);
    mWidget.Write(wb);
    wb.Write(mTextColorInt);
    wb.Write(mTextColorUint);
    wb.End();
}

// ── gcDoUISetTextColor::~gcDoUISetTextColor @ 0x003153f0 ──
gcDoUISetTextColor::~gcDoUISetTextColor() {
    *(void **)((char *)this + 4) = gcDoUISetTextColorvirtualtable;
}
