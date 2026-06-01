// exit  @ 0x001ac5c0  (gMain_psp.obj)
// Newlib-style C runtime exit: walks the per-reent _atexit chain calling each
// registered destructor (high index to low), runs the reent cleanup hook, then
// drops into _exit().

typedef void (*_exitfn_t)(void);

struct _atexit {
    struct _atexit *_next;   // +0x00
    int             _ind;    // +0x04
    _exitfn_t       _fns[32]; // +0x08 .. +0x88
};

struct _reent {
    char _pad0[0x3C];
    void (*_cleanup)(struct _reent *); // +0x3C
    char _pad1[0x148 - 0x3C - 4];
    struct _atexit *_atexit;  // +0x148
    struct _atexit  _atexit0; // +0x14C (embedded initial block)
};

extern "C" struct _reent *_impure_ptr;

#define _GLOBAL_REENT (_impure_ptr)

extern "C" void _exit(int);

extern "C" void exit(int code) {
    struct _atexit *p = _GLOBAL_REENT->_atexit;

    while (p != 0) {
        int n = p->_ind - 1;
        while (n >= 0) {
            (*p->_fns[n])();
            n--;
        }
        p = p->_next;
    }

    if (_GLOBAL_REENT->_cleanup != 0) {
        (*_GLOBAL_REENT->_cleanup)(_GLOBAL_REENT);
    }

    _exit(code);
}
