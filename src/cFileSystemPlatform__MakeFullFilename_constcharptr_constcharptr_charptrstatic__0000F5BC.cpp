// cFileSystemPlatform::MakeFullFilename(const char *, const char *, char *) static @ 0x0000F5BC
// Symbol: __0fTcFileSystemPlatformQMakeFullFilenamePCcTBPcT

extern "C" void *memcpy(void *dst, const void *src, unsigned int n);
extern "C" char *strcat(char *dst, const char *src);
extern "C" char *strcpy(char *dst, const char *src);
char *cStrFormat(char *, const char *, ...);

class cFileSystemPlatform {
public:
    static void MakeFullFilename(const char *dir, const char *name, char *out);
    static void AppendPath(char *dst, const char *path);
    static int IsCDDvD(void);
};

void cFileSystemPlatform::MakeFullFilename(const char *dir, const char *name, char *out)
{
    char buf[256];

    memcpy(buf, "a", 1);
    if (cFileSystemPlatform::IsCDDvD() != 0) {
        cStrFormat(out, "%s%s", "cd:");
        cFileSystemPlatform::AppendPath(buf, dir);
        cFileSystemPlatform::AppendPath(buf, name);
        strcat(out, buf);
    } else {
        strcpy(out, "ms0:");
        cFileSystemPlatform::AppendPath(buf, dir);
        cFileSystemPlatform::AppendPath(buf, name);
        strcat(out, buf + 1);
    }
}
