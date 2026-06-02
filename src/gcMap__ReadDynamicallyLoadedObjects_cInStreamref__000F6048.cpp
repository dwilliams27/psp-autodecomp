// gcMap::ReadDynamicallyLoadedObjects(cInStream &)  @ 0x000F6048  228B
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

struct gcDynObjLoadEntry {
    void *mType;        // 0x00
    int   mField4;      // 0x04
    cGUID mGuid;        // 0x08
    unsigned int mWord; // 0x10
};                      // size 0x14

// Global state referenced through relocations (masked in comparison).
extern int gNumDynObjLoad;                 // 0x0037D7F8
extern unsigned char *gTypeBase;           // 0x0037C068
extern gcDynObjLoadEntry gDynObjLoad[];    // 0x00099B18

class gcMap {
public:
    void ReadDynamicallyLoadedObjects(cInStream &s);
};

void gcMap::ReadDynamicallyLoadedObjects(cInStream &s) {
    s.Read(gNumDynObjLoad, 0x20, true);

    int i = 0;
    if (i < gNumDynObjLoad) {
        gcDynObjLoadEntry *e = &gDynObjLoad[i];
        do {
            unsigned int idx;
            s.Read(idx, 0x20, true);
            e->mType = (void *)(gTypeBase + idx * 0x30);
            e->mField4 = 0;
            e->mGuid.Read(s);
            s.Read(e->mWord, 0x20, true);
            i += 1;
            e += 1;
        } while (i < gNumDynObjLoad);
    }
}
