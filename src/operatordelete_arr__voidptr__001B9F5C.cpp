// operator delete[](void *)  @ 0x001b9f5c, 68B  (gMain_psp.obj)
// Emits __0OdlaPv
//
// SNC array-delete wrapper that pushes a single EH cleanup frame onto the
// global EH chain (head at 0x37E54C), forwards the pointer to the scalar
// operator delete(void*), then pops the frame.
//
// Frame layout (at sp+0):
//   +0  prev   = old chain head
//   +4  tag    = 2  (byte)
//   +8  zero
// The 128-byte frame is the SNC fixed EH unwind reserve.

extern int __eh_chain; // 0x37E54C : head of EH frame chain

struct eh_frame {
    int           prev; // +0
    unsigned char tag;  // +4  (=2)
    char          pad[3];
    int           zero; // +8
    char          reserve[0x70 - 0x0C]; // EH unwind reserve -> save area at +0x70
};

void operator delete(void *);

void operator delete[](void *p) {
    eh_frame f;
    f.prev = __eh_chain;
    __eh_chain = (int)&f;
    f.tag = 2;
    f.zero = 0;
    operator delete(p);
    __eh_chain = f.prev;
}
