// gcNamedSet::Reset(cMemPool *, bool)
// Address: 0x000d4e40, Size: 236B
// Obj: gcAll_psp.obj
// ODR-WARNING: split-TU local class declaration; do not add methods to include/*.h
//
// Global entity handle table at 0x38890: index = (handle & 0xFFFF), entry+0x30 holds
// the owning handle for validation. The cHandleT resolve is inlined THREE times:
//   (1) null-check the resolved enumeration object,
//   (2) re-resolve to test a +0x28 flag bit (8),
//   (3) re-resolve to pass as gcEnumeration::GetSize's this, whose result becomes the
//       second arg of an indirect destruct/reset callback fn(this+offset, size).

class cMemPool;

extern void *D_00038890[];

// gcEnumeration::GetSize(void) const  -> size in v0
extern "C" int __0fMgcEnumerationFGetSizeCv(const void *);

static inline void *ResolveHandle(int handle) {
    if (handle == 0) {
        return 0;
    }
    void *cand = D_00038890[handle & 0xFFFF];
    void *out = 0;
    if (cand != 0) {
        if (*(int *)((char *)cand + 0x30) == handle) {
            out = cand;
        }
    }
    return out;
}

struct DtorRec {
    short offset;       // +0x70 : member offset
    short pad;
    void (*fn)(void *, int);   // +0x74 : callback
};

struct gcNamedSet {
    void *mOwner;       // +0x00
    void *mClassDesc;   // +0x04 : pointer to class desc; +0x70 holds DtorRec
    int mHandle;        // +0x08
    void Reset(cMemPool *pool, bool flag);
};

void gcNamedSet::Reset(cMemPool *pool, bool flag) {
    int handle = this->mHandle;
    void *obj = ResolveHandle(handle);
    if (obj == 0) {
        return;
    }
    void *e = (handle != 0) ? D_00038890[handle & 0xFFFF] : 0;
    if ((unsigned char)((*(unsigned short *)((char *)e + 0x28) & 8) != 0)) {
        return;
    }
    DtorRec *rec = (DtorRec *)((char *)this->mClassDesc + 0x70);
    char *dst = (char *)this + rec->offset;
    void *raw = (handle != 0) ? D_00038890[handle & 0xFFFF] : 0;
    int size = __0fMgcEnumerationFGetSizeCv(raw);
    rec->fn(dst, size);
}
