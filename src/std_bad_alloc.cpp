// Mixed auxiliary functions from nwAll_psp.obj, cAll_psp.obj, eAll_psp.obj.
// Placed in a single TU as directed.

// ---- Function 3: cGetRandomSeed (cAll_psp.obj @ 0x00000e54, 28B) ----
extern unsigned int gRandomSeed1;  // linker-placed at 0x37BC14
extern unsigned int gRandomSeed2;  // linker-placed at 0x37BC10

extern "C" void cGetRandomSeed(unsigned int *seed1, unsigned int *seed2) {
    *seed1 = gRandomSeed1;
    *seed2 = gRandomSeed2;
}

// ---- Function 4: cCorePowerSave (cAll_psp.obj @ 0x00001d04, 28B) ----
extern unsigned char gCorePowerSave;  // linker-placed at 0x37C0A8

extern "C" void cCorePowerSave(bool enable) {
    if ((unsigned char)enable != gCorePowerSave) {
        gCorePowerSave = (unsigned char)enable;
    }
}

// ---- Function 5: eDynamicVertChunk::IsHandleValid (eAll_psp.obj @ 0x00011940, 28B) ----
class eDynamicChunkHandle {
public:
    unsigned short mId;
};

class eDynamicVertChunk {
public:
    static bool IsHandleValid(const eDynamicChunkHandle& h);
};

extern int gDynamicVertChunkCurrentId;  // linker-placed at 0x37D124

bool eDynamicVertChunk::IsHandleValid(const eDynamicChunkHandle& h) {
    return h.mId == (unsigned short)(gDynamicVertChunkCurrentId & 0xFFFF);
}

// ---- Function 1: std::bad_alloc::what(void) const (nwAll_psp.obj @ 0x0036b9c0, 24B) ----
// The lw/sw pair around the trivial return is the same pattern that matches
// std::exception::what() in src/std_exception.cpp — read-back-write of the
// __exception_ptr via a volatile store. Bytes are linker-resolved relocations
// (different return string than std::exception, but compare_func masks them).
extern int __exception_ptr;
extern const char _bad_alloc_str[];

namespace std {
    class bad_alloc {
    public:
        const char *what() const;
    };
}

const char *std::bad_alloc::what() const {
    int value = __exception_ptr;
    *(volatile int *)&__exception_ptr = value;
    return _bad_alloc_str;
}

// Function 2 (std::bad_alloc::~bad_alloc, 220B) is NOT matched in this TU.
// The destructor uses a two-level EH frame layout (push at sp+4 with tag 2,
// inner catch frame at sp+0x74 with tag 1 + typeinfo 0x37E4C8 + counter save
// at *0x37E548) that is SNC's private emit for nwAll_psp.obj's EH-aware build.
// This structure cannot be reproduced from portable C++; see session notes.
