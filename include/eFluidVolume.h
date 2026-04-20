#ifndef EFLUIDVOLUME_H
#define EFLUIDVOLUME_H

class cBase;
class cFile;
class cMemPool;
class cType;

class eFluidVolume {
public:
    cBase *mOwner;
    void *mClassDesc;
    cBase *mRef;
    float mField_C;
    float mField_10;

    eFluidVolume(cBase *);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

#endif
