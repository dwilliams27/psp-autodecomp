// gcExternalCinematic::gcExternalCinematic(cBase *) @ 0x000eae18
// gcExternalCinematic::Reset(cMemPool *, bool) @ 0x000eaee0
// gcExternalCinematic::Write(cFile &) const @ 0x000eace0
// gcExternalCinematic::New(cMemPool *, cBase *) static @ 0x00243958
// cObject base, gcCinematic embedded member at +0x44.

class cBase;
class cMemPool;
class cFile;

template <class T> T *dcast(const cBase *);

struct copy_word { int v; };

class cName {
public:
    void Set(const char *, ...);
};

class cObject {
public:
    cObject(cBase *);
    cObject &operator=(const cObject &);
    void Write(cFile &) const;
    char _cObjectPad[0x44];
};

class cBaseArray {
public:
    cBaseArray &operator=(const cBaseArray &);
};

class gcEvent {
public:
    gcEvent &operator=(const gcEvent &);
};

class gcExternalCinematic : public cObject {
public:
    gcExternalCinematic(cBase *);
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    void Reset(cMemPool *, bool);
};

class cWriteBlock {
public:
    cWriteBlock(cFile &, unsigned int);
    void End(void);
private:
    void *mFile;
    unsigned int mPos;
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

extern char gcExternalCinematicvirtualtable[];
extern void gcCinematic_gcCinematic(void *self, cBase *parent);

extern "C" {
    void gcExternalCinematic__gcExternalCinematic_cBaseptr(void *self, cBase *parent);
}

gcExternalCinematic::gcExternalCinematic(cBase *parent) : cObject(parent) {
    *(void **)((char *)this + 4) = gcExternalCinematicvirtualtable;
    gcCinematic_gcCinematic((char *)this + 0x44, (cBase *)this);
}

void gcExternalCinematic::Reset(cMemPool *, bool) {
    ((cName *)((char *)this + 0x4C))->Set((const char *)((char *)this + 8));

    /* Virtual call into the embedded gcCinematic at this+0x44, vtable
       entry 14 (offset 0x70).  SNC emits a manual vtable thunk: load vtable
       from this+0x48, fetch entry's this-adjust (lh) and func ptr (lw),
       compute adjusted this in the jalr delay slot. */
    struct VtblEntry { short adj; short pad; void (*func)(void *); };
    char *vtbl = *(char **)((char *)this + 0x48);
    VtblEntry *entry = (VtblEntry *)(vtbl + 0x70);
    short adj = entry->adj;
    void (*func)(void *) = entry->func;
    char *cin = (char *)this + 0x44;
    func(cin + adj);
}

// ── gcExternalCinematic::Write(cFile &) const @ 0x000eace0 ──
void gcExternalCinematic::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cObject::Write(file);

    /* Virtual call into the embedded gcCinematic at this+0x44, vtable
       entry at offset 0x28.  Same manual vtable thunk pattern as Reset. */
    struct VtblEntry { short adj; short pad; void (*func)(void *, cFile &); };
    char *vtbl = *(char **)((char *)this + 0x48);
    VtblEntry *entry = (VtblEntry *)(vtbl + 0x28);
    short adj = entry->adj;
    void (*func)(void *, cFile &) = entry->func;
    char *cin = (char *)this + 0x44;
    func(cin + adj, file);

    wb.End();
}

void gcExternalCinematic::AssignCopy(const cBase *src) {
    gcExternalCinematic *other = dcast<gcExternalCinematic>(src);
    cObject::operator=(*other);
    ((cObject *)((char *)this + 0x44))->operator=(*(cObject *)((char *)other + 0x44));
    ((cBaseArray *)((char *)this + 0x88))->operator=(
        *(const cBaseArray *)((char *)other + 0x88));
    *(unsigned char *)((char *)this + 0x90) = *(unsigned char *)((char *)other + 0x90);
    *(unsigned char *)((char *)this + 0x91) = *(unsigned char *)((char *)other + 0x91);
    unsigned char v92 = *(unsigned char *)((char *)other + 0x92);
    int *src94 = (int *)((char *)other + 0x94);
    *(unsigned char *)((char *)this + 0x92) = v92;
    int v94 = *src94;
    int *dst94 = (int *)((char *)this + 0x94);
    int *src98 = (int *)((char *)other + 0x98);
    *dst94 = v94;
    int *dst98 = (int *)((char *)this + 0x98);
    int *src9C = (int *)((char *)other + 0x9C);
    *dst98 = *src98;
    int *dst9C = (int *)((char *)this + 0x9C);
    int *srcA0 = (int *)((char *)other + 0xA0);
    *dst9C = *src9C;
    copy_word *srcA0W = (copy_word *)srcA0;
    copy_word *dstA0 = (copy_word *)((char *)this + 0xA0);
    *dstA0 = *srcA0W;
    ((gcEvent *)((char *)this + 0xA4))->operator=(*(const gcEvent *)((char *)other + 0xA4));
    *(int *)((char *)this + 0xC0) = *(int *)((char *)other + 0xC0);
}

// ── gcExternalCinematic::New(cMemPool *, cBase *) static @ 0x00243958 ──
cBase *gcExternalCinematic::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcExternalCinematic *result = 0;
    gcExternalCinematic *obj = (gcExternalCinematic *)rec->fn(base, 0xC4, 4, 0, 0);
    if (obj != 0) {
        gcExternalCinematic__gcExternalCinematic_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
