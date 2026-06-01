// _open_r @ 0x001b7e18 (gMain_psp.obj), 92B
// Newlib reentrant _open wrapper. Sibling of _close_r (0x001b7d10).
// Reconstructed to byte-exact SNC codegen.

typedef int s32;

extern "C" {
    int _open(const char *file, int flags, int mode);
    int _open_r(s32 *ptr, const char *file, int flags, int mode);
}

extern int errno;

int _open_r(s32 *ptr, const char *file, int flags, int mode)
{
    int ret;

    errno = 0;
    ret = _open(file, flags, mode);
    if (ret == -1) {
        if (errno != 0)
            *ptr = errno;
    }
    return ret;
}
