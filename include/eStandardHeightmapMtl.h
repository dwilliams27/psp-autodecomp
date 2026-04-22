#ifndef ESTANDARDHEIGHTMAPMTL_H
#define ESTANDARDHEIGHTMAPMTL_H

class cBase;
class cFile;
class cMemPool;

class eStandardHeightmapMtl {
public:
    eStandardHeightmapMtl(cBase *);
    void CreateData(void);
    void PlatformFree(void);
    void Unapply(void) const;
    void Write(cFile &) const;
    void PlatformRead(cFile &, cMemPool *);
    void AssignCopy(const cBase *);
    static eStandardHeightmapMtl *New(cMemPool *, cBase *);
};

#endif
