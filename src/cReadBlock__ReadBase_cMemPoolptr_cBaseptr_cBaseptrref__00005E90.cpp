// cReadBlock::ReadBase(cMemPool *, cBase *, cBase *&)  @ 0x00005e90, 108B
// obj: cAll_psp.obj  (split-TU isolated file)

class cMemPool;

struct cBaseSlot {
    short adj;
    void (*fn)(void *, void *, void *, short);
};

struct cBaseVtbl {
    char pad[0x30];
    cBaseSlot slot;
};

class cBase {
public:
    void *pad0;
    cBaseVtbl *vtbl;
};

class cReadBlock {
public:
    void *self0;
    cBase *CreateBase(cMemPool *, cBase *, cBase *&);
    void ReadBase(cMemPool *, cBase *, cBase *&);
};

void cReadBlock::ReadBase(cMemPool *pool, cBase *src, cBase *&out) {
    CreateBase(pool, src, out);

    cBase *base = out;
    if (base != 0) {
        void *arg = this->self0;
        cBaseSlot *slot = &base->vtbl->slot;
        short adj = slot->adj;
        slot->fn((char *)base + adj, arg, pool, adj);
    }
}
