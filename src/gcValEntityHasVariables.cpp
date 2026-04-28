// gcValEntityHasVariables — value type wrapping an entity reference plus a
// handle. Layout: gcValue base (~offset 8 .. 0x34), int field at 0x34.

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

class gcDesiredEntity {
public:
    gcDesiredEntity &operator=(const gcDesiredEntity &);
};

class gcValue {
public:
    void Write(cFile &) const;
};

class gcValEntityHasVariables {
public:
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
};

template <class T> T *dcast(const cBase *);

struct gcValVTEntry {
    short this_adjust;
    short pad;
    void (*fn)(void *, cFile *);
};

// ── gcValEntityHasVariables::AssignCopy(const cBase *) @ 0x003316d8 ──
void gcValEntityHasVariables::AssignCopy(const cBase *base) {
    gcValEntityHasVariables *other = dcast<gcValEntityHasVariables>(base);
    const gcDesiredEntity &src = *(const gcDesiredEntity *)(other + 8);
    *(gcDesiredEntity *)((char *)this + 8) = src;
    int &dst = *(int *)((char *)this + 0x34);
    const int &val = *(const int *)((char *)other + 0x34);
    dst = val;
}

// ── gcValEntityHasVariables::Write(cFile &) const @ 0x00331944 ──
void gcValEntityHasVariables::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcValue *)this)->Write(file);
    char *vt = *(char **)((char *)this + 0x0C);
    gcValVTEntry *entry = (gcValVTEntry *)(vt + 0x28);
    char *base = (char *)this + 8;
    ((void (*)(void *, cFile *))entry->fn)(base + entry->this_adjust,
                                           (cFile *)wb._data[0]);
    ((const cHandle *)((char *)this + 0x34))->Write(wb);
    wb.End();
}
