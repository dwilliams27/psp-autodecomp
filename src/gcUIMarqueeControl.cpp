// gcUIMarqueeControl — UI marquee control, derives from gcUITextControl.

class cBase;
class cFile;
class cMemPool;
class cType;

inline void *operator new(unsigned int, void *p) { return p; }

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

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

class gcUIControl : public gcUIWidget {
public:
    static cBase *New(cMemPool *, cBase *);
};

class gcUITextControl : public gcUIControl {
public:
    gcUITextControl(cBase *);
    ~gcUITextControl();
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcUIMarqueeControl : public gcUITextControl {
public:
    gcUIMarqueeControl(cBase *);

    const cType *GetType(void) const;
    bool IsUpdateEmpty(bool, bool) const;
    void Write(cFile &) const;
    ~gcUIMarqueeControl();

    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPoolHelper::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_0009990C;
extern cType *D_0009F40C;
extern cType *D_0009F410;
extern cType *D_0009F594;

// ── gcUIMarqueeControl::IsUpdateEmpty(bool, bool) const @ 0x0013C164 ──
// If `a` is true, the marquee considers itself non-empty. Otherwise it falls
// back to the base widget's IsUpdateEmpty.
bool gcUIMarqueeControl::IsUpdateEmpty(bool a, bool b) const {
    if (a) {
        return false;
    }
    return gcUIWidget::IsUpdateEmpty(a, b);
}

const cType *gcUIMarqueeControl::GetType(void) const {
    if (D_0009F594 == 0) {
        if (D_0009F410 == 0) {
            if (D_0009F40C == 0) {
                if (D_0009990C == 0) {
                    if (D_000385E0 == 0) {
                        if (D_000385DC == 0) {
                            D_000385DC = cType::InitializeType(
                                (const char *)0x36D894, (const char *)0x36D89C,
                                1, 0, 0, 0, 0, 0);
                        }
                        D_000385E0 = cType::InitializeType(
                            0, 0, 2, D_000385DC,
                            (cBase *(*)(cMemPool *, cBase *))0x1C3C58, 0, 0, 0);
                    }
                    D_0009990C = cType::InitializeType(
                        0, 0, 0x84, D_000385E0, 0, 0, 0, 0);
                }
                D_0009F40C = cType::InitializeType(
                    0, 0, 0x201, D_0009990C, gcUIControl::New, 0, 0, 0);
            }
            D_0009F410 = cType::InitializeType(
                0, 0, 0x200, D_0009F40C, gcUITextControl::New, 0, 0, 0);
        }
        D_0009F594 = cType::InitializeType(
            0, 0, 0xBF, D_0009F410, gcUIMarqueeControl::New, 0, 0, 0);
    }
    return D_0009F594;
}

// -- gcUIMarqueeControl::New(cMemPool *, cBase *) static @ 0x0029171c --
cBase *gcUIMarqueeControl::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcUIMarqueeControl *result = 0;
    gcUIMarqueeControl *obj =
        (gcUIMarqueeControl *)rec->fn(base, 0x124, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcUIMarqueeControl(parent);
        result = obj;
    }
    return (cBase *)result;
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
