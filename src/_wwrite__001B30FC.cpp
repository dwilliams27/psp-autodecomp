// _wwrite @ 0x001b30fc (gMain_psp.obj), 272 bytes
// libc-style wide write helper.

typedef signed int s32;
typedef unsigned int u32;
typedef unsigned short u16;
typedef signed char s8;

extern "C" s32 _vfwwrite(s32 fd, void *buf, s32 len, s32 flag);

extern "C" s32 _wwrite(s32 fd, void *src, u32 n)
{
    s32 total = 0;

    if (fd == 1 || fd == 2) {
        u32 chunk = 0x20;
        if (n < 0x20U) {
            chunk = n;
        }
        s32 i = 0;
        do {
            char buf[32];
            s32 j = 0;
            if (i < (s32)chunk) {
                u16 *p = (u16 *)src + i;
                do {
                    buf[j] = (s8)*p;
                    j += 1;
                    p += 1;
                    i += 1;
                } while (i < (s32)chunk);
            }
            s32 r = _vfwwrite(fd, buf, j, 0);
            s32 acc = total;
            total = acc + r;
            if (chunk == n) {
                break;
            }
            chunk += 0x20;
            if (r <= 0) {
                break;
            }
            if (n < chunk) {
                chunk = n;
            }
        } while (1);
    } else {
        total = (_vfwwrite(fd, src, n * 2, 0) / n) * 2;
    }
    return total;
}
