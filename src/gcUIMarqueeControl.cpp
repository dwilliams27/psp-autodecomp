// gcUIMarqueeControl — UI marquee control, derives from gcUITextControl.

class cBase;
class cFile;
class cMemPool;

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cMemPoolHelper {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(unsigned int);
    void End(void);
};

class gcUIWidget {
public:
    bool IsUpdateEmpty(bool, bool) const;
};

class gcUITextControl : public gcUIWidget {
public:
    gcUITextControl(cBase *);
    ~gcUITextControl();
    void Write(cFile &) const;
};

class gcUIMarqueeControl : public gcUITextControl {
public:
    bool IsUpdateEmpty(bool, bool) const;
    void Write(cFile &) const;
    ~gcUIMarqueeControl();

    static void operator delete(void *p) {
        cMemPool *pool = cMemPoolHelper::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

// ── gcUIMarqueeControl::IsUpdateEmpty(bool, bool) const @ 0x0013C164 ──
// If `a` is true, the marquee considers itself non-empty. Otherwise it falls
// back to the base widget's IsUpdateEmpty.
bool gcUIMarqueeControl::IsUpdateEmpty(bool a, bool b) const {
    if (a) {
        return false;
    }
    return gcUIWidget::IsUpdateEmpty(a, b);
}

// ── gcUIMarqueeControl::Write(cFile &) const @ 0x0013bdf0 ──
void gcUIMarqueeControl::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcUITextControl::Write(file);
    wb.Write(*(unsigned int *)((char *)this + 0x110));
    wb.Write(*(int *)((char *)this + 0x114));
    wb.Write(*(int *)((char *)this + 0x11C));
    wb.End();
}

// ── gcUIMarqueeControl::~gcUIMarqueeControl(void) @ 0x0013bfb0 ──
// Canonical C++ destructor. SNC's ABI auto-generates the (this != 0) guard,
// the chain call to ~gcUITextControl(), and the deleting-tail dispatch
// through operator delete.
extern char gcUIMarqueeControlvirtualtable[];

gcUIMarqueeControl::~gcUIMarqueeControl() {
    *(void **)((char *)this + 4) = gcUIMarqueeControlvirtualtable;
}
