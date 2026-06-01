// fclose  @ 0x001aa408  (gMain_psp.obj)
// Newlib-style fclose: CHECK_INIT(fp->_data) then flush + close the stream.

typedef int (*_close_fn_t)(void *);

struct __sFILE {
    unsigned char *_p;       // +0x00
    int            _r;        // +0x04
    int            _w;        // +0x08
    short          _flags;    // +0x0C
    short          _file;     // +0x0E
    int            _bf_base;  // +0x10
    int            _bf_size;  // +0x14
    int            _lbfsize;  // +0x18
    void          *_cookie;   // +0x1C
    void          *_read;     // +0x20
    void          *_write;    // +0x24
    void          *_seek;     // +0x28
    _close_fn_t    _close;    // +0x2C
    int            _ub[3];    // +0x30..+0x3C
    int            _up;       // +0x3C  (filler to push _data to +0x54)
    int            _ur;       // +0x40
    unsigned char  _ubuf[3];  // +0x44
    unsigned char  _nbuf[1];  // +0x47
    int            _lb_base;  // +0x48
    int            _lb_size;  // +0x4C
    int            _blksize;  // +0x50
    void          *_data;     // +0x54  (struct _reent *)
};

struct _reent {
    char _pad[0x38];
    int  __sdidinit;          // +0x38
};

extern "C" struct _reent *_impure_ptr;
extern "C" void __sinit(struct _reent *);
extern "C" int  fflush(struct __sFILE *);

#define _GLOBAL_REENT (_impure_ptr)

extern "C" int fclose(struct __sFILE *fp) {
    int r;

    if (fp == 0) {
        return 0;
    }

    if (fp->_data == 0) {
        fp->_data = _GLOBAL_REENT;
    }
    if (((struct _reent *)fp->_data)->__sdidinit == 0) {
        __sinit((struct _reent *)fp->_data);
    }

    if (fp->_flags == 0) {
        return 0;
    }

    r = 0;
    if (fp->_flags & 8) {
        r = fflush(fp);
    }
    if (fp->_close != 0 && (*fp->_close)(fp->_cookie) < 0) {
        r = -1;
    }
    fp->_flags = 0;
    return r;
}
