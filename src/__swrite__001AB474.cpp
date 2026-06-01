// __swrite @ 0x001ab474 (gMain_psp.obj), 148B
// Newlib reentrant stdio write hook. Sibling of __sread (0x1ab41c) / __sseek (0x1ab508).

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
    char _pad[0x50 - 0x30]; // 48..79
    int _offset;            // 80 (0x50)
    void *_data;            // 84 (0x54)
};

extern "C" {
    int _lseek_r(void *ptr, short fd, int offset, int whence);
    int _write_r(void *ptr, short fd, char *buf, int n);
    int __swrite(void *cookie, char *buf, int n);
}

int __swrite(void *cookie, char *buf, int n)
{
    struct __sFILE *fp = (struct __sFILE *)cookie;

    if (fp->_flags & 0x100)
        _lseek_r(fp->_data, fp->_file, 0, 2);
    fp->_flags = (short)(fp->_flags & ~0x1000);

    return _write_r(fp->_data, fp->_file, buf, n);
}
