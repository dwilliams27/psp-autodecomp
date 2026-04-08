#ifndef EKEYFRAMEDCONTROLLERTEMPLATE_H
#define EKEYFRAMEDCONTROLLERTEMPLATE_H

class eMesh;

template <class T>
class cHandleT {
public:
    int mIndex;
};

class eKeyframedControllerTemplate {
public:
    void SetMesh(cHandleT<eMesh>);
};

#endif
