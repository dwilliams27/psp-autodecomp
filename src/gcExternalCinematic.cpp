// gcExternalCinematic::gcExternalCinematic(cBase *) @ 0x000eae18
// gcExternalCinematic::Reset(cMemPool *, bool) @ 0x000eaee0
// gcExternalCinematic::Write(cFile &) const @ 0x000eace0
// gcExternalCinematic::New(cMemPool *, cBase *) static @ 0x00243958
// cObject base, gcCinematic embedded member at +0x44.

class cBase;
class cMemPool;
class cFile;

class cName {
public:
    void Set(const char *, ...);
};

class cObject {
public:
    cObject(cBase *);
    void Write(cFile &) const;
    char _cObjectPad[0x44];
};

class gcExternalCinematic : public cObject {
public:
    gcExternalCinematic(cBase *);
    static cBase *New(cMemPool *, cBase *);
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
