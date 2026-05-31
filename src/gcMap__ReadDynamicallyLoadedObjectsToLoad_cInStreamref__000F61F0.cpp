// gcMap::ReadDynamicallyLoadedObjectsToLoad(cInStream &)  @ 0x000F61F0  264B
// obj: gcAll_psp.obj
// Reads the dynamically-loaded-objects table from a stream.

class cInStream {
public:
    void Read(int &, int, bool);
    void Read(unsigned int &, int, bool);
};

class cGUID {
public:
    int a;
    int b;
    void Read(cInStream &);
};

class cHandle {
public:
    void Read(cInStream &);
};

struct gcDynObjLoadEntry {
    void *mType;        // 0x00
    int   mField4;      // 0x04
    cGUID mGuid;        // 0x08
    unsigned int mWord; // 0x10
};                      // size 0x14

// Global state referenced through relocations (masked in comparison).
extern int gNumDynObjLoad;                 // 0x0037D7F8
extern unsigned char *gTypeBase;           // 0x0037C068 (pointer to base of cType array)
extern gcDynObjLoadEntry gDynObjLoad[];    // 0x00099B18

class gcMap {
public:
    char pad_000[0x140];
    int mFlags;                            // 0x140
    char pad_144[0x3E4 - 0x144];
    cHandle mLoadBackgroundHandle;         // 0x3E4
public:
    void ReadDynamicallyLoadedObjectsToLoad(cInStream &s);
};

void gcMap::ReadDynamicallyLoadedObjectsToLoad(cInStream &s) {
    mLoadBackgroundHandle.Read(s);
    s.Read(gNumDynObjLoad, 0x20, true);

    int i = 0;
    if (i < gNumDynObjLoad) {
        do {
            gcDynObjLoadEntry *e = &gDynObjLoad[i];
            unsigned int idx;
            s.Read(idx, 0x20, true);
            e->mType = (void *)(gTypeBase + idx * 0x30);
            e->mField4 = 0;
            e->mGuid.Read(s);
            s.Read(e->mWord, 0x20, true);
            i += 1;
        } while (i < gNumDynObjLoad);
    }

    mFlags |= 0x40000;
}
