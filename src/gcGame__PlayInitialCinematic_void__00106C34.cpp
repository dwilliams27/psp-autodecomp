// gcGame::PlayInitialCinematic(void) @ 0x00106c34
// Symbol: __0fGgcGameUPlayInitialCinematicv
// Split-TU: local class declarations to avoid touching shared headers.

template <class T> class cGUIDT;
class gcStreamedCinematic;

class cStr {
public:
    char _data[256];
};

class gcMap {
public:
    int QueueStreamedCinematic(const cGUIDT<gcStreamedCinematic> &);
};

class gcGame {
public:
    unsigned int mFlags;   // accessed at +0x4CC
    int PlayInitialCinematic();
};

extern "C" int sscanf(const char *, const char *, ...);
void cGetCommandLineValue(const char *, cStr *);

extern gcMap *gMapSingleton;  // global at 0x37D7FC

int gcGame::PlayInitialCinematic() {
    cStr buf;

    if (!(*(unsigned int *)((char *)this + 0x4CC) & 1)) {
        return 0;
    }
    buf._data[0] = 0;
    cGetCommandLineValue("-initialcinematic", &buf);
    *(unsigned int *)((char *)this + 0x4CC) &= ~1u;

    int guid0 = 0;
    int guid1 = 0;
    sscanf(buf._data, "%x %x", &guid0, &guid1);

    if (gMapSingleton != 0) {
        gMapSingleton->QueueStreamedCinematic(*(const cGUIDT<gcStreamedCinematic> *)&guid0);
    }
    return 1;
}
