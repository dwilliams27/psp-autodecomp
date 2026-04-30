extern int __exception_ptr;
extern const char _exception_str[];
struct std_exception_frame {
    int prev;
    unsigned char tag;
    char pad[3];
    int zero;
    char rest[100];
};

namespace std {
    class exception {
    public:
        exception();
        const char* what() const;
    };
}

const char* std::exception::what() const {
    int value = __exception_ptr;
    *(volatile int *)&__exception_ptr = value;
    return _exception_str;
}

std::exception::exception() {
    std_exception_frame frame;
    int prev = __exception_ptr;
    __exception_ptr = (int)&frame;
    frame.prev = prev;
    __asm__ volatile("" ::: "memory");
    int tag = 2;
    register const int *vtable asm("a2");
    __asm__ volatile("lui $7,0x39");
    frame.tag = tag;
    __asm__ volatile("addiu %0,$7,-0x25e4" : "=r"(vtable));
    frame.zero = 0;
    int restore = *(volatile int *)&frame.prev;
    *(const int **)this = vtable;
    __exception_ptr = restore;
}
