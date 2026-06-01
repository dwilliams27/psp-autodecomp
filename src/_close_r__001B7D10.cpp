// _close_r @ 0x001b7d10 (gMain_psp.obj), 84B
// Newlib reentrant _close wrapper. Reconstructed to byte-exact SNC codegen.

typedef int s32;

extern "C" {
    int _close(int fd);
    int _close_r(s32 *ptr, int fd);
}

extern int errno;

int _close_r(s32 *ptr, int fd)
{
    int ret;

    errno = 0;
    ret = _close(fd);
    if (ret == -1) {
        if (errno != 0)
            *ptr = errno;
    }
    return ret;
}
