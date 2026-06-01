// atexit  @ 0x001ac4e0  (gMain_psp.obj)
// Newlib-style C runtime atexit: registers a destructor function pointer in
// the per-reent _atexit table, allocating a new overflow block when full.

typedef void (*_exitfn_t)(void);

struct _atexit {
    struct _atexit *_next;   // +0x00
    int             _ind;    // +0x04
    _exitfn_t       _fns[32]; // +0x08 .. +0x88
};

struct _reent {
    char _pad[0x148];
    struct _atexit *_atexit;  // +0x148
    struct _atexit  _atexit0; // +0x14C (embedded initial block)
};

extern "C" void *malloc(unsigned int);

extern "C" struct _reent *_impure_ptr;

#define _GLOBAL_REENT (_impure_ptr)

extern "C" int atexit(_exitfn_t fn) {
    struct _atexit *p = _GLOBAL_REENT->_atexit;

    if (p == 0) {
        p = &_GLOBAL_REENT->_atexit0;
        _GLOBAL_REENT->_atexit = p;
    }

    if (p->_ind >= 32) {
        p = (struct _atexit *)malloc(0x88);
        if (p == 0) {
            return -1;
        }
        p->_ind = 0;
        p->_next = _GLOBAL_REENT->_atexit;
        _GLOBAL_REENT->_atexit = p;
    }

    p->_fns[p->_ind++] = fn;
    return 0;
}
