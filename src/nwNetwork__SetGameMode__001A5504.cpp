// nwNetwork::SetGameMode(nwNetwork::nwGameMode) static
// Address: 0x001a5504, Size: 144B
// Obj: nwAll_psp.obj

class nwNetwork {
public:
    enum nwGameMode {};
    static void SetGameMode(nwGameMode);
};

struct nwLobby {
    short unk58_field;
};

struct nwSocketHandle {
    int id;
    nwSocketHandle() : id(0) {}
    nwSocketHandle(int v) : id(v) {}
};

extern "C" int nwSocket__GetSocket(nwSocketHandle handle);
extern "C" void nwSocket__Destroy(int sock);
extern "C" void **nwNetwork__GetLobby();

void nwNetwork::SetGameMode(nwGameMode mode) {
    nwSocketHandle *base = (nwSocketHandle *)0xA0000;
    int m = (int)mode;
    __asm__ volatile("" ::: "memory");
    if (nwSocket__GetSocket(base[-436]) != 0) {
        nwSocket__Destroy(nwSocket__GetSocket(base[-436]));
        nwSocketHandle empty;
        base[-436].id = empty.id;
    }
    *(int *)0x37D950 = m;
    if (m == 0) {
        if (nwNetwork__GetLobby() != 0) {
            void **lobby = nwNetwork__GetLobby();
            short *vt = (short *)((char *)*lobby + 0x58);
            short a1 = vt[0];
            void (*fn)(void *, short, void *) =
                *(void (**)(void *, short, void *))((char *)vt + 4);
            fn((char *)lobby + a1, a1, (void *)fn);
        }
    }
}
