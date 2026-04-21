#ifndef GCGENERICSUBSCRIBER_H
#define GCGENERICSUBSCRIBER_H

class cBase;
class cFile;

class gcGenericSubscriber {
public:
    void Attach(void);
    void Detach(void);
    int GetItem(int) const;
    int IsModifiable(void) const;
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
};

#endif
