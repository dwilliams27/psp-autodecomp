#ifndef ECONFIGBASE_H
#define ECONFIGBASE_H

class cBase;
class cMemPool;
class cType;

class eConfigBase {
public:
    ~eConfigBase();
    void AssignCopy(const cBase *);
    const char *GetImageFileFormatName(void) const;
    int GetPhysicsMemPoolSize(void) const;
    const cType *GetType(void) const;
    unsigned int GetAudioFrequency(bool) const;
    unsigned int GetAudioBytesPerSecond(unsigned int) const;
    static int FindResolution(int, int);
    static cBase *New(cMemPool *, cBase *);
};

#endif
