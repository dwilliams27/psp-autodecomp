// gcMap::IsDynamicallyLoadedObjectSetToLoad(const cGUID &) const  @ 0x000F62F8  120B
// obj: gcAll_psp.obj

class cGUID {
public:
    int a;
    int b;
};

struct gcDynObjLoadEntry {
    void *mType;        // 0x00
    int   mField4;      // 0x04
    cGUID mGuid;        // 0x08
    unsigned int mWord; // 0x10
};                      // size 0x14

extern int gNumDynObjLoad;                 // 0x0037D7F8
extern gcDynObjLoadEntry gDynObjLoad[];    // 0x00099B18

class gcMap {
public:
    bool IsDynamicallyLoadedObjectSetToLoad(const cGUID &g) const;
};

bool gcMap::IsDynamicallyLoadedObjectSetToLoad(const cGUID &g) const {
    int i = 0;
    if (i < gNumDynObjLoad) {
        do {
            cGUID *e = &gDynObjLoad[i].mGuid;
            int found = 0;
            if (e->a == g.a) {
                if (e->b == g.b) {
                    found = 1;
                }
            }
            if ((unsigned char)found) {
                return true;
            }
            i += 1;
        } while (i < gNumDynObjLoad);
    }
    return false;
}
