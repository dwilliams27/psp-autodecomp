// vfprintf @ 0x001af56c — newlib nano vfprintf wrapper.
// Scans format string for a floating-point conversion; dispatches to
// _vfprintf_r (float-capable) or _vfiprintf_r (integer-only).
#pragma control sched=2

struct _reent;
typedef char *va_list_t;

struct __FILE {
    char pad[0x54];
    struct _reent *_data;  // offset 0x54
};

extern "C" int _vfprintf_r(struct _reent *, __FILE *, const char *, va_list_t);
extern "C" int _vfiprintf_r(struct _reent *, __FILE *, const char *, va_list_t);

extern "C" int vfprintf(__FILE *fp, const char *fmt0, va_list_t ap) {
    const char *fmt = fmt0;
    char c;

    while ((c = *fmt) != 0) {
        if (c == '%') {
            if (*(fmt + 1) == 0)
                goto cont;
            fmt++;
            c = *fmt;
            while (c < 'A' && *(fmt + 1) != 0) {
                fmt++;
                c = *fmt;
            }
            switch (c) {
            case 'E': case 'F': case 'G':
            case 'e': case 'f': case 'g':
                return _vfprintf_r(fp->_data, fp, fmt0, ap);
            default:
                break;
            }
        }
    cont:
        fmt++;
    }
    return _vfiprintf_r(fp->_data, fp, fmt0, ap);
}
