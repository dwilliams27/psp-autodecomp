// gcExternalCinematic::gcExternalCinematic(cBase *) @ 0x000eae18
// gcExternalCinematic::Reset(cMemPool *, bool) @ 0x000eaee0
// cObject base, gcCinematic embedded member at +0x44.

class cBase;
class cMemPool;

class cName {
public:
    void Set(const char *, ...);
};

class cObject {
public:
    cObject(cBase *);
    char _cObjectPad[0x44];
};

class gcExternalCinematic : public cObject {
public:
    gcExternalCinematic(cBase *);
    void Reset(cMemPool *, bool);
};

extern char gcExternalCinematicvirtualtable[];
extern void gcCinematic_gcCinematic(void *self, cBase *parent);

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
