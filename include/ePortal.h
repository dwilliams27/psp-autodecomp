#ifndef EPORTAL_H
#define EPORTAL_H

class cBase;
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

class cObject {
public:
    cObject(cBase *);
    ~cObject(void);
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
};

class ePortal : public cObject {
public:
    ePortal(cBase *);
    ~ePortal(void);
    void Activate(bool);
    void DisconnectRoom(cHandleT<eRoom>, bool);
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
};

#endif
