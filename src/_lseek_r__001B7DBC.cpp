// _lseek_r @ 0x001b7dbc (gMain_psp.obj), 92B
// Newlib reentrant _lseek wrapper. SNC byte-exact reconstruction.
//
// Disassembly: a0=ptr, a1=fd, a2=offset, a3=whence.
//   global errno (0x9F994) cleared to 0
//   r = _lseek(fd, offset, whence)
//   if (r == -1 && errno != 0) *ptr = errno;
//   return r;
//
// Sibling of _fstat_r / _close_r (same reent pattern).

typedef int s32;

extern "C" {
    int _lseek(int fd, int offset, int whence);
    int _lseek_r(s32 *ptr, int fd, int offset, int whence);
}

extern int errno;

int _lseek_r(s32 *ptr, int fd, int offset, int whence)
{
    int ret;

    errno = 0;
    ret = _lseek(fd, offset, whence);
    if (ret == -1) {
        if (errno != 0) {
            *ptr = errno;
        }
    }
    return ret;
}
