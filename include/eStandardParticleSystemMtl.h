#ifndef ESTANDARDPARTICLESYSTEMMTL_H
#define ESTANDARDPARTICLESYSTEMMTL_H

class cBase;
class cFile;
class cMemPool;

class eStandardParticleSystemMtl {
public:
    void CreateData(void);
    void PlatformFree(void);
    void PlatformRead(cFile &, cMemPool *);
    void Unapply(void) const;
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static eStandardParticleSystemMtl *New(cMemPool *, cBase *);
};

#endif
