// _fopen_r @ 0x001aa838 (gMain_psp.obj), 260B
// Newlib reentrant fopen. Reconstructed to byte-exact SNC codegen.

typedef short s16;
typedef int s32;

struct __sFILE {
    unsigned char *_p;      // 0
    int _r;                 // 4
    int _w;                 // 8
    short _flags;           // 12 (0xC)
    short _file;            // 14 (0xE)
    int _bf_base;           // 16 (0x10)
    int _bf_size;           // 20 (0x14)
    int _lbfsize;           // 24 (0x18)
    void *_cookie;          // 28 (0x1C)
    int (*_read)();         // 32 (0x20)
    int (*_write)();        // 36 (0x24)
    int (*_seek)();         // 40 (0x28)
    int (*_close)();        // 44 (0x2C)
    char _pad[0x54 - 0x30]; // 48..83
    void *_data;            // 84 (0x54)
};

extern "C" {
    short __sflags(void *ptr, const char *mode, int *oflags);
    struct __sFILE *__sfp(void *ptr);
    short _open_r(void *ptr, const char *file, int oflags, int mode);
    int fseek(struct __sFILE *fp, long offset, int whence);

    int __sread();
    int __swrite();
    int __sseek();
    int __sclose();

    void *_fopen_r(void *ptr, const char *file, const char *mode);
}

void *_fopen_r(void *ptr, const char *file, const char *mode)
{
    struct __sFILE *fp;
    int f;
    int oflags;
    short flags;

    flags = __sflags(ptr, mode, &oflags);
    if (flags == 0)
        return 0;

    fp = __sfp(ptr);
    if (fp == 0)
        return 0;

    f = _open_r(fp->_data, file, oflags, 0666);
    if (f < 0) {
        fp->_flags = 0;
        return 0;
    }

    fp->_flags = flags;
    fp->_file = (short)f;
    fp->_cookie = fp;
    fp->_read = __sread;
    fp->_write = __swrite;
    fp->_seek = __sseek;
    fp->_close = __sclose;

    if (fp->_flags & 0x100) {
        fseek(fp, 0, 2);
    }

    return fp;
}
