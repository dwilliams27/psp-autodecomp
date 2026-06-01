// _read_r @ 0x001b7e74 (gMain_psp.obj), 92B
// Newlib reentrant _read wrapper. SNC byte-exact reconstruction.
//
// Disassembly: a0=ptr, a1=fd, a2=buf, a3=len.
//   global errno (0x9F994) cleared to 0
//   r = _read(fd, buf, len)
//   if (r == -1 && errno != 0) *ptr = errno;
//   return r;
//
// Sibling of _lseek_r / _fstat_r / _close_r / _open_r (same reent pattern).

typedef int s32;

extern "C" {
    int _read(int fd, void *buf, int len);
    int _read_r(s32 *ptr, int fd, void *buf, int len);
}

extern int errno;

int _read_r(s32 *ptr, int fd, void *buf, int len)
{
    int ret;

    errno = 0;
    ret = _read(fd, buf, len);
    if (ret == -1) {
        if (errno != 0) {
            *ptr = errno;
        }
    }
    return ret;
}
