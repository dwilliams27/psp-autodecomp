#ifndef GCVALMOUSESTATE_H
#define GCVALMOUSESTATE_H

class cBase;
class cFile;
class cMemPool;

class gcValMouseState {
public:
    ~gcValMouseState(void);
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    void VisitReferences(unsigned int, cBase *,
        void (*)(cBase *, unsigned int, void *),
        void *, unsigned int);

    int pad0;
    int pad4;
    int pad8;
    int padC;
};

#endif
