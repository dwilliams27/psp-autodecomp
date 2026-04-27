// eLensFlareSprite — eAll_psp.obj
//   0x001e8378  AssignCopy(const cBase *)                 (96B)

class cBase;
class cFile;
class cMemPool;
class cWriteBlock;

class cHandle {
public:
    int mId;
    void Write(cWriteBlock &) const;
};

class eLensFlareSprite {
public:
    cBase *mOwner;
    void *mClassDesc;
    int mField8;
    cHandle mFieldC;
    float mField10;
    float mField14;
    float mField18;
    float mField1C;

    void AssignCopy(const cBase *);
};

template <class T> T *dcast(const cBase *);

// 0x001e8378 — AssignCopy(const cBase *), 96B
void eLensFlareSprite::AssignCopy(const cBase *base) {
    eLensFlareSprite *o = dcast<eLensFlareSprite>(base);
    mField8 = o->mField8;
    mFieldC = o->mFieldC;
    mField10 = o->mField10;
    mField14 = o->mField14;
    mField18 = o->mField18;
    mField1C = o->mField1C;
}
