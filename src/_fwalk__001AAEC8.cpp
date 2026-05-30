// _fwalk @ 0x001aaec8 (gMain_psp.obj) — newlib-style stdio file walk
// Walks the glue chain reachable from (char*)reent + 0x1D8, invoking the
// callback on each FILE whose halfword flags field (offset 12) is nonzero.

struct _walk_file {
    char    _pad[12];
    short   _flags;     // offset 12 — loaded as halfword
    char    _rest[0x58 - 14];
};

struct _walk_glue {
    struct _walk_glue *_next;   // offset 0
    int                _niobs;  // offset 4
    struct _walk_file *_iobs;   // offset 8
};

extern "C" int _fwalk(int reent, int (*func)(void *))
{
    struct _walk_glue *g;
    struct _walk_file *fp;
    int n;
    int ret = 0;

    g = (struct _walk_glue *)(reent + 0x1D8);
    if (g) {
        do {
            for (fp = g->_iobs, n = g->_niobs - 1; n >= 0; n--, fp++) {
                if (fp->_flags != 0) {
                    ret |= (*func)((void *)fp);
                }
            }
        } while ((g = g->_next) != 0);
    }
    return ret;
}
