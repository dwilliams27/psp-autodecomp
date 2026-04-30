class eWorld;

extern "C" void *__vec_new(void *array, int count, int size, void (*ctor)(void *));
extern "C" void *memset(void *, int, unsigned int);

class eCameraBins {
public:
    const eWorld *mWorld;
    unsigned short mBinCount;
    char _pad06[2];
    int mField08;
    char _pad0C[0x17700];
    unsigned char mFlag1770C;
    char _pad1770D[3];
    int mField17710;
    int mField17714;
    char _pad17718[0x18];
    char mArray17730[4][0x310];
    int mField18370;
    char mClear18374[0x8C];
    unsigned char mFlag18400;

    eCameraBins(unsigned short, const eWorld *);
};

// eCameraBins::eCameraBins(unsigned short, const eWorld *) @ 0x000307ec
eCameraBins::eCameraBins(unsigned short binCount, const eWorld *world) {
    mWorld = world;
    mBinCount = binCount;
    mField08 = 0;
    mFlag1770C = 0;
    mField17710 = 2;
    mField17714 = 0;
    __vec_new(mArray17730, 4, 0x310, (void (*)(void *))0x2D864);
    mField18370 = 0;
    mFlag18400 = 0;
    memset(mClear18374, 0, 0x8C);
}
