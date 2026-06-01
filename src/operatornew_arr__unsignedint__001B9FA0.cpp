// operator new[](unsigned int) @ 0x001b9fa0 (gMain_psp.obj)
// mangled: __0OnwaUi
//
// SNC/ProDG array operator-new. Pushes a vec-new EH guard entry onto the
// global EH stack (head at g_eh_stack_top), forwards the request to the
// scalar operator new(unsigned int) with the array EH info, then pops the
// guard entry and returns the allocation.

struct an_eh_stack_entry {
    an_eh_stack_entry *next;   // 0
    unsigned char kind;        // 4
    void *info;                // 8
};

// Global EH stack head (masked relocation in compare).
extern an_eh_stack_entry *g_eh_stack_top;

// Array-alloc EH info object referenced by the guard entry.
extern int g_array_new_eh_info;

// Scalar operator new. In this build SNC forwards the EH info + kind as
// trailing argument registers (relocation/identity masked in compare).
extern void *operator_new_scalar(unsigned int size, void *info, int kind);

struct eh_frame {
    an_eh_stack_entry entry;   // 0..12
    char scratch[100];         // pad to fixed 112-byte EH scratch area
};

void *operator new[](unsigned int size)
{
    eh_frame f;
    f.entry.next = g_eh_stack_top;
    g_eh_stack_top = &f.entry;
    int kind = 2;
    f.entry.kind = (unsigned char)kind;
    f.entry.info = &g_array_new_eh_info;
    void *p = operator_new_scalar(size, &g_array_new_eh_info, kind);
    g_eh_stack_top = f.entry.next;
    return p;
}
