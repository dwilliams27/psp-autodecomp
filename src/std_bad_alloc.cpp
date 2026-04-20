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

// Function 1 (std::bad_alloc::what, 24B) and Function 2 (std::bad_alloc::~bad_alloc,
// 220B) are NOT matched in this TU. Both rely on Sony stdlib's EH-frame emit
// (load/store of the *0x37E54C exception chain around the function body) that
// cannot be reproduced from plain C++. See session notes for details.
