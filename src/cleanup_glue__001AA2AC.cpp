// cleanup_glue @ 0x001aa2ac (gMain_psp.obj), 76 bytes
// Recursive free helper: walks a singly-linked list via the first word,
// freeing children before the node itself.

extern "C" void _free_r(int reent, void *ptr);

extern "C" void cleanup_glue(int reent, void **node) {
    void *next = *node;
    if (next != 0) {
        cleanup_glue(reent, (void **)next);
    }
    _free_r(reent, node);
}
