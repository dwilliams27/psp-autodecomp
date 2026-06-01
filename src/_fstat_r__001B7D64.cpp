// _fstat_r @ 0x001b7d64 (gMain_psp.obj), 88B
// Newlib reentrant _fstat wrapper. SNC byte-exact reconstruction.
//
// Disassembly: a0=ptr, a1=fd, a2=st.
//   global errno cleared to 0
//   r = _fstat(fd, st)
//   if (r == -1 && errno != 0) *ptr = errno;
//
// ptr points directly at a reent _errno field (offset 0).

typedef int s32;

extern "C" {
    int _fstat(int fd, void *st);
    void _fstat_r(s32 *ptr, int fd, void *st);
    extern int errno;
}

void _fstat_r(s32 *ptr, int fd, void *st)
{
    errno = 0;
    if (_fstat(fd, st) == -1) {
        s32 e = errno;
        if (e != 0) {
            *ptr = e;
        }
    }
}
