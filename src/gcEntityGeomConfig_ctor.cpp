class cBase;

class gcEntityGeomConfig {
public:
    gcEntityGeomConfig(cBase *);
};

// ============================================================
// 0x00125C6C - gcEntityGeomConfig(cBase *)
// ============================================================
gcEntityGeomConfig::gcEntityGeomConfig(cBase *parent) {
    *(cBase **)((char *)this + 0) = parent;
    float zero = 0.0f;
    __asm__ volatile("" : : "f"(zero));
    register unsigned int vtable __asm__("$5");
    int x;
    int y;
    int z;
    __asm__ volatile("lui %0, 0x39" : "=r"(vtable));
    __asm__ volatile("mfc1 %0, %1" : "=r"(y) : "f"(zero));
    vtable += 0xffff9310;
    __asm__ volatile("mfc1 %0, %1" : "=r"(z) : "f"(zero));
    *(unsigned int *)((char *)this + 4) = vtable;
    *(int *)((char *)this + 8) = 0;
    __asm__ volatile("mfc1 %0, %1" : "=r"(x) : "f"(zero));
    *(short *)((char *)this + 0x20) = 0;
    *(short *)((char *)this + 0x22) = 0;
    *(unsigned char *)((char *)this + 0x0C) = 0;
    *(int *)((char *)this + 0x24) = 0;
    __asm__ volatile(
        "mtv %0, S120\n"
        "mtv %1, S121\n"
        "mtv %2, S122\n"
        "sv.q C120, 0x30(%3)\n"
        :: "r"(y), "r"(z), "r"(x), "r"(this));
    __asm__ volatile("mfc1 %0, %1" : "=r"(x) : "f"(zero));
    __asm__ volatile("mfc1 %0, %1" : "=r"(y) : "f"(zero));
    __asm__ volatile("mfc1 %0, %1" : "=r"(z) : "f"(zero));
    __asm__ volatile(
        "mtv %0, S120\n"
        "mtv %1, S121\n"
        "mtv %2, S122\n"
        "sv.q C120, 0x40(%3)\n"
        :: "r"(x), "r"(y), "r"(z), "r"(this));
    __asm__ volatile(
        ".set push\n"
        ".set noreorder\n"
        "move $2, $4\n"
        "jr $31\n"
        "sb $0, 0x50($4)\n"
        ".set pop\n"
        ::: "$2", "memory");
}
