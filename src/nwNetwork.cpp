void cStrCopy(char *, const char *, int);

struct VtEntry {
    short adjust;
    short pad;
    int (*fn)(void *);
};

struct VtObject {
    VtEntry *vtable;
};

class cMemAllocator;

class cMemPool {
public:
    cMemPool(cMemAllocator *, const char *);
};

class cDynamicMemAllocator {
public:
    cDynamicMemAllocator(const char *, unsigned int, unsigned int, void *);
};

class cTimeValue {
public:
    int mTime;
};

class nwStatsTracking {
public:
    static VtObject *Get(void);
};

class nwHeadset {
public:
    static void Update(void);
};

enum nwTransportType {};

struct nwNetworkConfig {
    char pad0[0xC];
    unsigned short port;
    char padE[2];
    int capacity;
};

class nwSocket {
public:
    void Destroy(void);
    static int Create(nwTransportType, unsigned short, unsigned int, int, int);
    static nwSocket *GetSocket(int);
    static void UpdateAll(cTimeValue);
};

extern "C" void __record_needed_destruction(void *);
extern "C" void cMemPool_ctor(cMemPool *, cMemAllocator *, const char *)
    asm("__0oIcMemPoolctP6NcMemAllocatorPCc");
extern "C" void cDynamicMemAllocator_ctor(cDynamicMemAllocator *, const char *,
                                          unsigned int, unsigned int, void *)
    asm("__0oUcDynamicMemAllocatorctPCcUiTCPv");
extern int D_00034C5C asm("D_00034C5C");
extern char D_00034C60[] asm("D_00034C60");
extern char D_00000328[] asm("D_00000328");
extern nwNetworkConfig *volatile D_0037D94C asm("D_0037D94C");
extern volatile int D_0009F930 asm("D_0009F930");

class nwNetwork {
public:
    enum nwNetError {};
    static cMemPool *GetMemPool(void);
    static int Initialize(void);
    static VtObject *GetLobby(void);
    static void Update(cTimeValue);
    static int CreateGameSocket(int, unsigned short);
    static int GetGameVersion(void);
    static int GetLastPlatformError(void);
    static void GetInterfaceName(int, char *, int);
    static void CheckSecurity(void);
    static void StopInterface(void);
    static int GetSelectedInterface(void);
    static void StartInterface(void);
    static int GetNumInterfaces(void);
    static int IsInterfaceValid(int);
    static void SetLastError(nwNetError);
    static int PlatformInitialize(void);
    static int PlatformUpdate(cTimeValue);
    static nwTransportType PlatformGetGameTransport(void);
};

int nwNetwork::GetLastPlatformError(void) {
    return 0;
}

void nwNetwork::GetInterfaceName(int, char *dest, int size) {
    cStrCopy(dest, (const char *)0x371198, size);
}

void nwNetwork::CheckSecurity(void) {
    SetLastError(nwNetError(0));
    *(unsigned char *)0x37D949 = 1;
}

void nwNetwork::StopInterface(void) {
    SetLastError(nwNetError(0));
    *(unsigned char *)0x37D948 = 0;
    *(unsigned char *)0x37D949 = 0;
}

void nwNetwork::StartInterface(void) {
    SetLastError(nwNetError(0));
    *(unsigned char *)0x37D948 = 1;
    *(unsigned char *)0x37D949 = 0;
}

int nwNetwork::GetSelectedInterface(void) {
    int selected = -1;
    if (GetNumInterfaces() > 0) {
        selected = 0;
    }
    return selected;
}

int nwNetwork::IsInterfaceValid(int index) {
    int valid = 0;
    if (index >= 0 && index < GetNumInterfaces()) {
        valid = 1;
    }
    return valid & 0xff;
}

int nwNetwork::Initialize(void) {
    SetLastError(nwNetError(0));
    GetMemPool();
    if (PlatformInitialize() == 0) {
        return 0;
    }

    VtObject *lobby = GetLobby();
    if (lobby != 0) {
        VtEntry *entry = (VtEntry *)((char *)lobby->vtable + 8);
        if (entry->fn((char *)lobby + entry->adjust) == 0) {
            return 0;
        }
    }

    VtObject *stats = nwStatsTracking::Get();
    if (stats != 0) {
        VtEntry *entry = (VtEntry *)((char *)stats->vtable + 8);
        if (entry->fn((char *)stats + entry->adjust) == 0) {
            return 0;
        }
    }

    return 1;
}

void nwNetwork::Update(cTimeValue dt) {
    if (PlatformUpdate(dt) != 0) {
        if (*(int *)0x37D950 != 0) {
            VtObject *lobby = GetLobby();
            if (lobby != 0) {
                VtEntry *entry = (VtEntry *)((char *)lobby->vtable + 0x18);
                typedef void (*Fn)(void *, cTimeValue);
                ((Fn)entry->fn)((char *)lobby + entry->adjust, dt);
            }

            VtObject *stats = nwStatsTracking::Get();
            if (stats != 0) {
                VtEntry *entry = (VtEntry *)((char *)stats->vtable + 0x18);
                typedef void (*Fn)(void *, cTimeValue);
                ((Fn)entry->fn)((char *)stats + entry->adjust, dt);
            }

            nwHeadset::Update();
            nwSocket::UpdateAll(dt);
        }
    }
}

int nwNetwork::CreateGameSocket(int maxConnections, unsigned short port) {
    volatile int created;
    unsigned short actualPort = port;
    if (actualPort == 0xFFFF) {
        actualPort = D_0037D94C->port;
    }

    if (nwSocket::GetSocket(D_0009F930) != 0) {
        nwSocket::GetSocket(D_0009F930)->Destroy();
    }

    nwTransportType transport = PlatformGetGameTransport();
    created =
        nwSocket::Create(transport, actualPort, GetGameVersion(), maxConnections,
                         D_0037D94C->capacity);
    D_0009F930 = created;
    return D_0009F930;
}

cMemPool *nwNetwork::GetMemPool(void) {
    cMemPool *pool = (cMemPool *)D_00034C60;
    cMemPool *ret = pool;
    if (D_00034C5C == 0) {
        D_00034C5C = 1;
        void *config = *(void **)0x37D94C;
        cDynamicMemAllocator *allocator =
            (cDynamicMemAllocator *)((char *)pool + 0x9C);
        unsigned int configSize = *(unsigned int *)((char *)config + 0x10);
        unsigned int doubled = configSize + configSize;
        unsigned int size = doubled + 0x13000;
        cMemPool_ctor(pool, (cMemAllocator *)allocator, (const char *)0x37118C);
        *(void **)((char *)pool + 0x98) = D_00000328;
        cDynamicMemAllocator_ctor(allocator, (const char *)pool + 4, size, 0, 0);
        __record_needed_destruction((void *)0x37D970);
    }
    return ret;
}
