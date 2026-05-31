// __sfp  @ 0x001aa74c  (gMain_psp.obj)
// newlib stdio file-pointer allocator. Split-TU local declarations.

struct __sFILE {
    char *_p;            // 0x00
    int   _r;            // 0x04
    int   _w;            // 0x08
    short _flags;        // 0x0C
    short _file;         // 0x0E
    int   _bf_base;      // 0x10
    int   _bf_size;      // 0x14
    int   _lbfsize;      // 0x18
    char  _pad1[0x30 - 0x1C];
    int   _ub_base;      // 0x30
    int   _ub_size;      // 0x34
    char  _pad2[0x44 - 0x38];
    int   _ur;           // 0x44
    int   _up;           // 0x48
    char  _pad3[0x54 - 0x4C];
    void *_data;         // 0x54
};

struct _glue {
    _glue   *_next;      // 0x00
    int      _niobs;     // 0x04
    __sFILE *_iobs;      // 0x08
};

struct _reent {
    int    _errno;       // 0x00
    char   _pad0[0x38 - 0x04];
    int    __sdidinit;   // 0x38
    char   _pad1[0x1D8 - 0x3C];
    _glue  __sglue;      // 0x1D8
};

extern "C" void __sinit(_reent *);
extern "C" _glue *__sfmoreglue(_reent *, int);

extern "C" __sFILE *__sfp(_reent *d)
{
    __sFILE *fp;
    int n;
    _glue *g;

    if (d->__sdidinit == 0)
        __sinit(d);

    g = &d->__sglue;
    for (;;) {
        for (fp = g->_iobs, n = g->_niobs; --n >= 0; fp++)
            if (fp->_flags == 0)
                goto found;

        if (g->_next == 0 && (g->_next = __sfmoreglue(d, 4)) == 0)
            break;
        g = g->_next;
    }

    d->_errno = 0xC;
    return 0;

found:
    fp->_flags = 1;
    fp->_p = 0;
    fp->_w = 0;
    fp->_r = 0;
    fp->_bf_base = 0;
    fp->_bf_size = 0;
    fp->_lbfsize = 0;
    fp->_file = -1;
    fp->_ub_base = 0;
    fp->_ub_size = 0;
    fp->_ur = 0;
    fp->_up = 0;
    fp->_data = d;
    return fp;
}
