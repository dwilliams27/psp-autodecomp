// __throw_setup @ 0x001bad68 (gMain_psp.obj), 112 bytes
// Free function. Reconstructed signatures from call targets:
//   eh_alloc_on_stack(unsigned int)
//   push_throw_stack(a_type_info_impl*, void(*)(void*,int), unsigned char,
//                    unsigned char*, char*, int, void*, unsigned char*,
//                    a_throw_stack_entry*)

struct a_type_info_impl;
struct a_throw_stack_entry;

typedef void (*type_cb_t)(void *, int);

// a0 holds the a_type_info_impl*; field at 0xC is a callback pointer.
struct a_type_info_impl {
    char pad0[0xC];
    type_cb_t cb;   // 0x0C
};

extern void *eh_alloc_on_stack(unsigned int size);
extern void push_throw_stack(a_type_info_impl *ti, type_cb_t cb, unsigned char flags,
                             unsigned char *a3, char *a4, int a5, void *a6,
                             unsigned char *a7, a_throw_stack_entry *a8);

extern "C" void *__throw_setup(a_type_info_impl *ti, unsigned int size, int flags)
{
    type_cb_t cb = ti->cb;
    void *p = eh_alloc_on_stack(size);
    push_throw_stack(ti, cb, (unsigned char)flags, 0, 0, 0, p, 0, 0);
    return p;
}
