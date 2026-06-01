// nwSocket::Create(void)
// Address: 0x001a81b4, Size: 172B
// Obj: nwAll_psp.obj
//
// Allocates and zeroes the connection-pointer array (mNumConnections entries)
// via the network mem-pool's allocator virtual call.

extern "C" void *memset(void *, int, unsigned int);

struct VtEntry {
    short adjust;
    short pad;
    void *(*fn)(void *, unsigned int, int, int, int);
};

struct VtObject {
    char pad0[0x1c];
    VtEntry *vtable; // 0x1c
};

class cMemPool {
public:
    char pad0[0x24];
    VtObject *allocator; // 0x24
};

class nwNetwork {
public:
    static cMemPool *GetMemPool(void);
};

class nwSocket {
public:
    int mPad0;             // 0x00
    int mPad4;             // 0x04
    int mNumConnections;   // 0x08
    void *mConnections;    // 0x0C

    int Create(void);
};

int nwSocket::Create(void) {
    if (mNumConnections == 0) {
        return 1;
    }

    VtObject *alloc = nwNetwork::GetMemPool()->allocator;
    int size = mNumConnections * 4;
    VtEntry *entry = (VtEntry *)((char *)alloc->vtable + 0x28);
    void *mem = entry->fn((char *)alloc + entry->adjust, size, 0, 0, 0);
    memset(mem, 0, size);
    mConnections = mem;
    return mem != 0;
}
