#ifndef EBIPEDCONTROLLERTEMPLATE_H
#define EBIPEDCONTROLLERTEMPLATE_H

class eMesh;

template <class T>
class cHandleT {
public:
    int mIndex;
};

class eBipedControllerTemplate {
public:
    void SetMesh(cHandleT<eMesh>);
};

#endif
