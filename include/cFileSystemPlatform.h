#ifndef CFILESYSTEMPLATFORM_H
#define CFILESYSTEMPLATFORM_H

struct mVec3;

class cFileSystemPlatform {
public:
    static int IsCDDvD(void);
};

class eMoviePlatform {
public:
    void read_delete(void);
};

class eSoundData {
public:
    void CreateData(void);
};

class eBspTree {
public:
    void PlatformFree(void);
    void SetHullData(int, const mVec3 *, int, const unsigned short *, unsigned int);
};

#endif
