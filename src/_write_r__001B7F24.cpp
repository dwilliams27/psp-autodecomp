// _write_r @ 0x001b7f24 (gMain_psp.obj), 92B
// Newlib reentrant _write wrapper. Sibling of _open_r (0x001b7e18, 92B).
// Reconstructed to byte-exact SNC codegen.

typedef int s32;

extern "C" {
    int _write(int fd, const void *buf, int nbytes);
    int _write_r(s32 *ptr, int fd, const void *buf, int nbytes);
}

extern int errno;

int _write_r(s32 *ptr, int fd, const void *buf, int nbytes)
{
    int ret;

    errno = 0;
    ret = _write(fd, buf, nbytes);
    if (ret == -1) {
        if (errno != 0)
            *ptr = errno;
    }
    return ret;
}
