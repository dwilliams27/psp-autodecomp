// __sseek @ 0x001ab508 (gMain_psp.obj), 92B
// Newlib reentrant stdio seek hook. Sibling of __sread (0x1ab41c).

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
    int __sseek(void *cookie, int offset, int whence);
}

int __sseek(void *cookie, int offset, int whence)
{
    struct __sFILE *fp = (struct __sFILE *)cookie;
    int ret;

    ret = _lseek_r(fp->_data, fp->_file, offset, whence);
    if (ret == -1)
        fp->_flags = (short)(fp->_flags & ~0x1000);
    else {
        fp->_flags = (short)(fp->_flags | 0x1000);
        fp->_offset = ret;
    }

    return ret;
}
