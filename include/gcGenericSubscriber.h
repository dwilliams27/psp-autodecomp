#ifndef GCGENERICSUBSCRIBER_H
#define GCGENERICSUBSCRIBER_H

class cBase;
class cFile;
class cMemPool;
class cType;

class gcGenericSubscriber {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Attach(void);
    void Detach(void);
    int GetItem(int) const;
    int IsModifiable(void) const;
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
};

#endif
