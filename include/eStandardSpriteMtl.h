#ifndef ESTANDARDSPRITEMTL_H
#define ESTANDARDSPRITEMTL_H

class cBase;
class cFile;
class cMemPool;
class cType;

class eStandardSpriteMtl {
public:
    eStandardSpriteMtl(cBase *);
    const cType *GetType(void) const;
    void CreateData(void);
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    void PlatformRead(cFile &, cMemPool *);
    void PlatformFree(void);
    void Unapply(void) const;
    static eStandardSpriteMtl *New(cMemPool *, cBase *);
};

#endif
