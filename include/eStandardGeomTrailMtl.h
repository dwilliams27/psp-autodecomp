#ifndef ESTANDARDGEOMTRAILMTL_H
#define ESTANDARDGEOMTRAILMTL_H

class cBase;
class cFile;
class cMemPool;
class cType;

class eStandardGeomTrailMtl {
public:
    eStandardGeomTrailMtl(cBase *);
    const cType *GetType(void) const;
    void PlatformFree(void);
    void PlatformRead(cFile &, cMemPool *);
    void Unapply(void) const;
    void CreateData(void);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static eStandardGeomTrailMtl *New(cMemPool *, cBase *);
};

#endif
