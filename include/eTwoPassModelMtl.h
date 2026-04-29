#ifndef ETWOPASSMODELMTL_H
#define ETWOPASSMODELMTL_H

class cBase;
class cFile;
class cMemPool;
class eDrawInfo;
class mOCS;
class eColor;
class cType;

class eTwoPassModelMtl {
public:
    eTwoPassModelMtl(cBase *);
    const cType *GetType(void) const;
    void PlatformFree(void);
    void Unapply(void) const;
    void CreateData(void);
    void ApplyDynamic(const eDrawInfo &, const mOCS &, float, unsigned int, eColor) const;
    void PlatformRead(cFile &, cMemPool *);
    void Write(cFile &) const;
    static eTwoPassModelMtl *New(cMemPool *, cBase *);
};

#endif
