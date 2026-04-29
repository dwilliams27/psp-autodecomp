#ifndef ESTATICLIGHT_H
#define ESTATICLIGHT_H

class cBase;
class cFile;
class cType;

class eStaticLight {
public:
    eStaticLight(cBase *);
    ~eStaticLight();
    void Write(cFile &) const;
    const cType *GetType(void) const;
    int GetNumStratifiedSamples(void) const;
};

#endif
