// gcDesiredUIWidget — gcAll_psp.obj
//   0x0012ec88  Write(cFile &) const                       (88B)
//   0x0012ece0  Read(cFile &, cMemPool *)                  (200B)
//   0x0012eda8  GetObject(bool) const                      (28B)
//   0x0012edc4  GetText(char *) const                      (28B)
//   0x0027a3d8  New(cMemPool *, cBase *) static            (148B)

class cObject;
class cBase;
class cFile;
class cMemPool;

inline void *operator new(unsigned int, void *p) { return p; }

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

void cFile_SetCurrentPos(void *, unsigned int);

class gcDesiredObject {
public:
    cBase *mOwner;
    void *mVTable;
    unsigned int mNext;

    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

extern "C" void gcDesiredObject_ctor(void *self, cBase *parent);

struct gcDesiredUIWidgetHelper {
    int _a;
    int _b;
    int _c;
    enum gcPrimary { gcPrimaryDef = 1 };
    void Write(cWriteBlock &) const;
    void GetText(char *) const;
    void Read(cReadBlock &);
};

extern "C" void gcDesiredUIWidgetHelper_ctor(void *self, gcDesiredUIWidgetHelper::gcPrimary p);

class gcDesiredUIWidget {
public:
    cObject *Get(bool) const;
    cObject *GetObject(bool) const;
    void GetText(char *) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    static cBase *New(cMemPool *, cBase *);
};

extern char gcDesiredUIWidgetvirtualtable[]; // 0x38A3C0

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

cObject *gcDesiredUIWidget::GetObject(bool b) const {
    return Get(b);
}

void gcDesiredUIWidget::GetText(char *buf) const {
    ((gcDesiredUIWidgetHelper *)((char *)this + 0xC))->GetText(buf);
}

// 0x0012ec88 — Write(cFile &) const
void gcDesiredUIWidget::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcDesiredObject *)this)->Write(file);
    ((const gcDesiredUIWidgetHelper *)((const char *)this + 0xC))->Write(wb);
    wb.End();
}

// 0x0012ece0 — Read(cFile &, cMemPool *)
int gcDesiredUIWidget::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && ((gcDesiredObject *)this)->Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    ((gcDesiredUIWidgetHelper *)((char *)this + 0xC))->Read(rb);
    return result;
}

// 0x0027a3d8 — New(cMemPool *, cBase *) static
cBase *gcDesiredUIWidget::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDesiredUIWidget *result = 0;
    gcDesiredUIWidget *obj = (gcDesiredUIWidget *)entry->fn(base, 0x18, 4, 0, 0);
    if (obj != 0) {
        gcDesiredObject_ctor(obj, parent);
        *(void **)((char *)obj + 4) = gcDesiredUIWidgetvirtualtable;
        gcDesiredUIWidgetHelper_ctor((char *)obj + 12, gcDesiredUIWidgetHelper::gcPrimaryDef);
        result = obj;
    }
    return (cBase *)result;
}
