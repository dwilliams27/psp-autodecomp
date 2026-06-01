// __placement_array_new @ 0x001b9c1c (gMain_psp.obj)
// Compiler-runtime thunk: placement array operator-new helper.
// Forwards to array_new_general, passing through the element destructor and
// the secondary (placement) destructor, with the placement flag (arg10 = 1)
// and the global allocation-policy pointer as the trailing stack argument.
//
// Sibling of __vec_new (0x001b9c64), which forwards the same way but with a
// single destructor, no placement-destructor argument, and the placement
// flag cleared.

typedef void (*dtor_fn)(void *);
typedef void (*dtor2_fn)(void *, int);
typedef void *(*alloc_fn)(unsigned int);

extern void *array_new_general(void *, int, unsigned int, void *, dtor_fn,
                               dtor2_fn, alloc_fn, void *, int, int, int,
                               unsigned int);

// Global allocation-policy pointer (masked relocation in compare).
extern void *g_vec_new_policy;

// __placement_array_new is a compiler runtime symbol emitted unmangled by SNC;
// the DB entry is literally "__placement_array_new", so it must be defined
// with C linkage.
extern "C" void *__placement_array_new(void *mem, int count, int size,
                                       dtor_fn dtor, dtor2_fn dtor2) {
    return array_new_general(mem, count, (unsigned int)size, 0, dtor, dtor2, 0,
                             0, 0, 1, 0, (unsigned int)g_vec_new_policy);
}
