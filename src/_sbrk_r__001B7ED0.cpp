/* _sbrk_r @ 0x001b7ed0  (gMain_psp.obj)
 * newlib reentrant sbrk: clears errno, calls _sbrk(incr), and on failure
 * copies the (non-zero) global errno into the reent struct's _errno slot. */

typedef int s32;

extern "C" {

s32 _sbrk(s32 incr);

/* Global errno at 0x9F994 (lui 0xa; -1644). */
extern s32 g_errno;

void _sbrk_r(s32 *reent, s32 incr) {
    g_errno = 0;
    if (_sbrk(incr) == -1) {
        s32 e = g_errno;
        if (e != 0) {
            *reent = e;
        }
    }
}

}
