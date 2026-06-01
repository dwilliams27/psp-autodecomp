// write @ 0x001c08a8 (gMain_psp.obj), 104B
// SN Systems libc write() dispatch shim:
//   fd 1 (stdout) or 2 (stderr) -> host _write (SN debug console)
//   otherwise                   -> _sn_syscall(5, fd, buf, nbytes)  (kernel write)

extern "C" {
    int _write(int fd, const void *buf, int nbytes);
    int _sn_syscall(int num, int a, int b, int c);
    int write(int fd, const void *buf, int nbytes);
}

int write(int fd, const void *buf, int nbytes)
{
    if (fd == 2 || fd == 1)
        return _write(fd, buf, nbytes);

    return _sn_syscall(5, fd, (int)buf, nbytes);
}
