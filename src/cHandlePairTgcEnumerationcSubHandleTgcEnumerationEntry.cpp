// Decompiled functions from gcAll_psp.obj
//   0x00234318 cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> >::cHandlePairT(const cHandlePairT &)
//   0x0022757c cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> >::~cHandlePairT(void)
//   0x0025aa9c gcEntityAnimationConfigSet::GetSize(void) const
//   0x0025c880 gcEntityAttackSet::GetSize(void) const

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

extern "C" {

void *cMemPool_GetPoolFromPtr(void *);
void free(void *);

// 0x00234318 — copy constructor, copies two ints and returns this
void *cHandlePairT_gcEnumeration_cSubHandleT_gcEnumerationEntry____cHandlePairT_copy__00234318(
        void *self, const void *other) {
    ((int *)self)[0] = ((const int *)other)[0];
    ((int *)self)[1] = ((const int *)other)[1];
    return self;
}

// 0x0022757c — destructor (empty body, only handles flags & 1 delete path)
void cHandlePairT_gcEnumeration_cSubHandleT_gcEnumerationEntry_____dtor__0022757C(
        void *self, int flags) {
    if (self != 0 && (flags & 1)) {
        void *pool = cMemPool_GetPoolFromPtr(self);
        if (pool != 0) {
            void *block = *(void **)((char *)pool + 0x24);
            DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
            short off = rec->offset;
            rec->fn((char *)block + off, self);
        } else {
            free(self);
        }
    }
}

// 0x0025aa9c — GetSize: read array length from header word at [-1]
int gcEntityAnimationConfigSet__GetSize_voidconst__0025AA9C(const void *self) {
    const int *arr = *(const int *const *)((const char *)self + 12);
    int size = 0;
    if (arr != 0) {
        size = arr[-1];
    }
    return size;
}

// 0x0025c880 — same GetSize shape as above, different class
int gcEntityAttackSet__GetSize_voidconst__0025C880(const void *self) {
    const int *arr = *(const int *const *)((const char *)self + 12);
    int size = 0;
    if (arr != 0) {
        size = arr[-1];
    }
    return size;
}

}
