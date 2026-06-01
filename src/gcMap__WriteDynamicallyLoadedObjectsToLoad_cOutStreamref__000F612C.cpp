// gcMap::WriteDynamicallyLoadedObjectsToLoad(cOutStream &)  @ 0x000F612C  196B
// obj: gcAll_psp.obj
// Writes the dynamically-loaded-objects table to a stream.
// Counterpart of gcMap::ReadDynamicallyLoadedObjectsToLoad.

class cOutStream {
public:
    void Write(int, int, bool);
    void Write(unsigned int, int, bool);
};

class cGUID {
public:
    int a;
    int b;
    void Write(cOutStream &) const;
};

class cHandle {
public:
    void Write(cOutStream &) const;
};

struct cType {
    void *mField0;       // 0x00
    int   mTypeId;       // 0x04
};

struct gcDynObjLoadEntry {
    cType *mType;        // 0x00
    int    mField4;      // 0x04
    cGUID  mGuid;        // 0x08
    unsigned int mWord;  // 0x10
};                       // size 0x14

// Global state referenced through relocations (masked in comparison).
extern int gNumDynObjLoad;                 // 0x0037D7F8
extern gcDynObjLoadEntry gDynObjLoad[];    // 0x00099B18

class gcMap {
public:
    char pad_000[0x3E4];
    cHandle mLoadBackgroundHandle;         // 0x3E4
public:
    void WriteDynamicallyLoadedObjectsToLoad(cOutStream &s);
};

void gcMap::WriteDynamicallyLoadedObjectsToLoad(cOutStream &s) {
    mLoadBackgroundHandle.Write(s);
    s.Write(gNumDynObjLoad, 0x20, true);

    int i = 0;
    if (i < gNumDynObjLoad) {
        do {
            gcDynObjLoadEntry *e = &gDynObjLoad[i];
            s.Write((unsigned int)e->mType->mTypeId, 0x20, true);
            e->mGuid.Write(s);
            s.Write(e->mWord, 0x20, true);
            i += 1;
        } while (i < gNumDynObjLoad);
    }
}
