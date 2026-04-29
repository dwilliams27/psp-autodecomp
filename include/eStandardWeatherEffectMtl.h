#ifndef ESTANDARDWEATHEREFFECTMTL_H
#define ESTANDARDWEATHEREFFECTMTL_H

class cFile;
class cBase;
class cMemPool;

class eStandardWeatherEffectMtl {
public:
    void PlatformFree(void);
    void Unapply(void) const;
    void CreateData(void);
    void PlatformRead(cFile &, cMemPool *);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static eStandardWeatherEffectMtl *New(cMemPool *, cBase *);
};

#endif
