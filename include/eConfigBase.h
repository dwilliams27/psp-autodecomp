#ifndef ECONFIGBASE_H
#define ECONFIGBASE_H

class cBase;
class cMemPool;

class eConfigBase {
public:
    const char *GetImageFileFormatName(void) const;
    int GetPhysicsMemPoolSize(void) const;
    unsigned int GetAudioBytesPerSecond(unsigned int) const;
    static int FindResolution(int, int);
    static cBase *New(cMemPool *, cBase *);
};

#endif
