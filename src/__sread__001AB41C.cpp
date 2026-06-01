// __sread @ 0x001ab41c (gMain_psp.obj), 88B
// Newlib reentrant stdio read hook. Reconstructed to byte-exact SNC codegen.

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
    int _read_r(void *ptr, short fd, char *buf, int n);
    int __sread(void *cookie, char *buf, int n);
}

int __sread(void *cookie, char *buf, int n)
{
    struct __sFILE *fp = (struct __sFILE *)cookie;
    int ret;

    ret = _read_r(fp->_data, fp->_file, buf, n);
    if (ret >= 0)
        fp->_offset += ret;
    else
        fp->_flags = (short)(fp->_flags & ~0x1000);

    return ret;
}
