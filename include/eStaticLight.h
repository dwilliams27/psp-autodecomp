#ifndef ESTATICLIGHT_H
#define ESTATICLIGHT_H

class cBase;
class cFile;

class eStaticLight {
public:
    eStaticLight(cBase *);
    ~eStaticLight();
    void Write(cFile &) const;
    int GetNumStratifiedSamples(void) const;
};

#endif
