// vsnprintf @ 0x001ac408 — newlib bounded string-print wrapper.
// Builds a string-write FILE on the stack (_flags = __SWR|__SSTR = 0x208,
// _w/_bf._size = size, _data = global reent), forwards to vfprintf, then
// null-terminates _p. Clamps the result to -1 when size < ret.
#pragma control sched=2

struct _reent;
typedef char *va_list_t;

struct __FILE {
    unsigned char *_p;     // +0  next char ptr
    int _r;                // +4
    int _w;                // +8  remaining width
    short _flags;          // +12
    short _file;           // +14
    struct {
        unsigned char *_base;  // +16
        int _size;             // +20
    } _bf;
    char _pad[60];         // +24 .. fill to +84
    struct _reent *_data;  // +84
};

extern struct _reent *_impure_ptr;

extern "C" int vfprintf(__FILE *fp, const char *fmt, va_list_t ap);

extern "C" int vsnprintf(char *str, unsigned int size, const char *fmt, va_list_t ap) {
    __FILE f;
    f._flags = 0x208;
    f._p = (unsigned char *)str;
    f._bf._base = (unsigned char *)str;
    f._w = (int)size;
    f._bf._size = (int)size;
    f._data = _impure_ptr;
    unsigned int ret = (unsigned int)vfprintf(&f, fmt, ap);
    *f._p = '\0';
    if (size < ret) {
        ret = (unsigned int)-1;
    }
    return (int)ret;
}
