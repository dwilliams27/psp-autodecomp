#ifndef ESTANDARDSPRITEMTL_H
#define ESTANDARDSPRITEMTL_H

class cBase;
class cMemPool;

class eStandardSpriteMtl {
public:
    void CreateData(void);
    void PlatformFree(void);
    void Unapply(void) const;
    static eStandardSpriteMtl *New(cMemPool *, cBase *);
};

#endif
