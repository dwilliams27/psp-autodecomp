#ifndef EPORTAL_H
#define EPORTAL_H

#include "cObject.h"

class eRoom;

template <class T>
class cHandleT {
public:
    int mIndex;
};

class cMemPool {
public:
    static void *GetPoolFromPtr(const void *);
};

class ePortal : public cObject {
public:
    ePortal(cBase *);
    ~ePortal(void);
    const cType *GetType(void) const;
    void Activate(bool);
    void ConnectRoom(cHandleT<eRoom>, bool);
    void DisconnectRoom(cHandleT<eRoom>, bool);
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

#endif
