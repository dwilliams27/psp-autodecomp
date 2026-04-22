#ifndef EKEYFRAMEDCONTROLLERTEMPLATE_H
#define EKEYFRAMEDCONTROLLERTEMPLATE_H

class eMesh;
class cBase;
class cFile;
class cMemPool;

template <class T>
class cHandleT {
public:
    int mIndex;
};

class eKeyframedControllerTemplate {
public:
    eKeyframedControllerTemplate(cBase *);
    void SetMesh(cHandleT<eMesh>);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static eKeyframedControllerTemplate *New(cMemPool *, cBase *);
};

#endif
