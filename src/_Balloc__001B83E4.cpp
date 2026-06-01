// _Balloc @ 0x001b83e4 (gMain_psp.obj), 196 bytes
// newlib mprec.c Balloc — Bigint freelist allocator.

typedef unsigned long ULong;

struct _Bigint {
    struct _Bigint *_next;
    int _k, _maxwds, _sign, _wds;
    ULong _x[1];
};

struct _reent_partial {
    char _pad[0x4C];
    struct _Bigint **_freelist;
};

extern "C" void *_calloc_r(void *ptr, unsigned int n, unsigned int size);

extern "C" struct _Bigint *_Balloc(struct _reent_partial *ptr, int k)
{
    int x;
    struct _Bigint *rv;

    if (ptr->_freelist == 0) {
        ptr->_freelist =
            (struct _Bigint **)_calloc_r(ptr, sizeof(struct _Bigint *), 16);
        if (ptr->_freelist == 0)
            return 0;
    }
    if ((rv = ptr->_freelist[k]) != 0) {
        ptr->_freelist[k] = rv->_next;
    } else {
        x = 1 << k;
        rv = (struct _Bigint *)_calloc_r(ptr, 1,
            sizeof(struct _Bigint) + (x - 1) * sizeof(ULong));
        if (rv == 0)
            return 0;
        rv->_k = k;
        rv->_maxwds = x;
    }
    rv->_sign = rv->_wds = 0;
    return rv;
}
