// cFileSystemPlatform::Open(cFilePlatform *) static @ 0x0000F240
// Symbol: __0fTcFileSystemPlatformEOpenP6NcFilePlatformT

class cFilePlatform {
public:
    char pad0[0x108];   // 0x000..0x107
    int  m_108;         // 0x108
};

class cFileSystemPlatform {
public:
    static int Open(cFilePlatform *f);
    static int OpenFromPack(cFilePlatform *f);
    static void OpenFromDisk(cFilePlatform *f);
};

int cFileSystemPlatform::Open(cFilePlatform *f)
{
    if (f->m_108 >= 0) {
        return 1;
    }
    if (cFileSystemPlatform::OpenFromPack(f) == 0) {
        cFileSystemPlatform::OpenFromDisk(f);
    }
    if (f->m_108 < 0) {
        return 0;
    }
    return 1;
}
