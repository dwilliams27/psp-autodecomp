#ifndef CFILESYSTEMPLATFORM_H
#define CFILESYSTEMPLATFORM_H

struct mVec3;

class cFilePlatform;

class cFileSystemPlatform {
public:
    static int IsCDDvD(void);
    static int FileExists(const char *name);
    static int GetFileStatus(void);
    static void SetSuspendCallback(void (*)(void *), void *);
    static void ReplaceChar(char *str, char from, char to);
    static int ExitCallback(int, int, void *);
    static void Close(cFilePlatform *fp);
    static void WaitSuspend(void);
    static void WaitAsync(cFilePlatform *fp);
};

class eMoviePlatform {
public:
    char _pad0[0x2D8];
    unsigned int dispbuf_pts[2];     // 0x2D8
    int dispbuf_writeIdx;            // 0x2E0
    int dispbuf_readIdx;             // 0x2E4
    int dispbuf_buf;                 // 0x2E8
    int dispbuf_bufEnd;              // 0x2EC
    char _pad1[0x28];               // 0x2F0 - 0x318
    unsigned int soundbuf_pts[5];    // 0x318
    int soundbuf_readIdx;            // 0x32C
    int soundbuf_buf;                // 0x330
    int soundbuf_bufEnd;             // 0x334

    void read_delete(void);
    int dispbuf_getCapacity(void);
    void dispbuf_setPts(unsigned int);
    int dispbuf_getPts(void);
    int soundbuf_getCapacity(void);
    int soundbuf_getPts(void);
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
