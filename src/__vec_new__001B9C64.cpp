// __vec_new @ 0x001b9c64 (gMain_psp.obj)
// Compiler-runtime thunk: array operator-new helper.
// Forwards to array_new_general with the default fill/policy arguments,
// passing the element destructor through and supplying the global
// allocation policy pointer as the trailing stack argument.

typedef void (*dtor_fn)(void *);
typedef void (*dtor2_fn)(void *, int);
typedef void *(*alloc_fn)(unsigned int);

extern void *array_new_general(void *, int, unsigned int, void *, dtor_fn,
                               dtor2_fn, alloc_fn, void *, int, int, int,
                               unsigned int);

// Global allocation-policy pointer (masked relocation in compare).
extern void *g_vec_new_policy;

// __vec_new is a compiler runtime symbol emitted unmangled by SNC; the DB
// entry is literally "__vec_new", so it must be defined with C linkage.
extern "C" void *__vec_new(void *mem, int count, int size, dtor_fn dtor) {
    return array_new_general(mem, count, (unsigned int)size, 0, dtor, 0, 0, 0,
                             0, 0, 0, (unsigned int)g_vec_new_policy);
}
