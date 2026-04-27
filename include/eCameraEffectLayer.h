#ifndef ECAMERAEFFECTLAYER_H
#define ECAMERAEFFECTLAYER_H

class cBase;
class cFile;
class cMemPool;
class cType;
class cWriteBlock;

class cHandle {
public:
    int mId;
    void Write(cWriteBlock &) const;
};

class eCameraEffectLayer {
public:
    cBase *mOwner;
    void *mClassDesc;
    bool mField8;
    int mFieldC;
    cHandle mField10;

    eCameraEffectLayer(cBase *);
    ~eCameraEffectLayer();
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    static void operator delete(void *);
    const cType *GetType(void) const;
};

#endif
