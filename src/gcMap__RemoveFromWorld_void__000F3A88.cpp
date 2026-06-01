// gcMap::RemoveFromWorld(void)  @ 0x000F3A88  200B
// obj: gcAll_psp.obj
// split-TU: local class decl, do not touch shared src/gcMap.cpp

class gcMap;

extern "C" void eWeatherSystem__RemoveFromWorld_void(void *self);          // 0x61EDC
extern "C" void gcEntity__DestroyAllDynamic_void(void);                    // 0x1168EC
extern "C" int  gcRegionBase__RemoveFromWorld_void(void *self);           // 0xEED5C

struct DispatchSlot {
    short offset;
    short _pad;
    void (*fn)(void *);
};

class gcMap {
public:
    char  pad_000[0x1F4];
    void *mWeather;     // 0x1F4
    char  pad_1F8[0x4]; // 0x1F8
    void *mField1FC;    // 0x1FC  object with class descriptor at +4
    char  pad_200[0x10];
    void *mRegions[2];  // 0x210, 0x214
public:
    int RemoveFromWorld(void);
};

int gcMap::RemoveFromWorld(void) {
    eWeatherSystem__RemoveFromWorld_void(mWeather);

    void *obj = mField1FC;
    DispatchSlot *slot = (DispatchSlot *)(*(char **)((char *)obj + 4) + 0xC8);
    slot->fn((char *)obj + slot->offset);
    *(int *)((char *)obj + 0x64) = 0;

    gcEntity__DestroyAllDynamic_void();

    int i = 0;
    gcMap *p = this;
    for (; i < 2; i++) {
        void *r = p->mRegions[0];
        if (r != 0) {
            DispatchSlot *rs = (DispatchSlot *)(*(char **)((char *)r + 4) + 0xC8);
            rs->fn((char *)r + rs->offset);
        }
        p = (gcMap *)((char *)p + 4);
    }

    if (gcRegionBase__RemoveFromWorld_void(this) != 0) {
        return 1;
    }
    return 0;
}
