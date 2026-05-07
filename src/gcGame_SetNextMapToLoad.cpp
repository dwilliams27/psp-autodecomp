// ODR-WARNING: split-TU local redeclaration for one gcGame method. Do not
// widen include/gcGame.h for isolated decompilation targets.

class gcMap;
class gcLoadingScreen;
class gcRegion;

template <class T>
class cGUIDT {
public:
    int mB;
    int mA;
};

class gcGame {
public:
    void SetNextMapToLoad(const cGUIDT<gcMap> &,
                          const cGUIDT<gcLoadingScreen> &,
                          const cGUIDT<gcRegion> *,
                          unsigned int);
};

extern "C" int cIRand(void);
extern cGUIDT<gcMap> D_00099B08[];

void gcGame::SetNextMapToLoad(const cGUIDT<gcMap> &map,
                              const cGUIDT<gcLoadingScreen> &loadingScreen,
                              const cGUIDT<gcRegion> *regions,
                              unsigned int token) {
    register gcGame *self __asm__("s0") = this;
    *(cGUIDT<gcMap> *)((char *)self + 0x16F0) = map;
    *(cGUIDT<gcLoadingScreen> *)((char *)self + 0x16F8) = loadingScreen;

    cGUIDT<gcRegion> empty;
    int offset = 0;
    char *pending = (char *)D_00099B08 + offset;
    int count = 0;
    char *src = (char *)regions + offset;
    do {
        char *use;
        if (regions == 0) {
            empty.mA = 0;
            empty.mB = 0;
            use = (char *)&empty;
        } else {
            use = src;
        }
        *(unsigned int *)pending = *(int *)use;
        *(int *)(4 + pending) = *(int *)(use + 4);
        pending += 8;
        count += 1;
        src += 8;
    } while (count < 2);

    unsigned int result;
    if (token != 0) {
        result = token;
    } else {
        result = cIRand() | 1;
    }
    *(unsigned int *)((char *)self + 0x16EC) = result;
}
