// cFileSystemPlatform::AppendPath(char *, const char *) static @ 0x0000F6C8
// Symbol: __0fTcFileSystemPlatformKAppendPathPcPCcT

extern "C" char *strcat(char *dst, const char *src);
extern "C" char *strupr(char *s);

class cFileSystemPlatform {
public:
    static void AppendPath(char *dst, const char *path);
    static int IsCDDvD(void);
    static void ReplaceChar(char *s, char from, char to);
};

void cFileSystemPlatform::AppendPath(char *dst, const char *path)
{
    char buf[256];

    if (path != 0 && *path != 0) {
        buf[0] = 0;
        if (*path != '/' && *path != '\\') {
            *(short *)buf = '/';
        }
        strcat(buf, path);
        if (cFileSystemPlatform::IsCDDvD() != 0) {
            strupr(buf);
        }
        cFileSystemPlatform::ReplaceChar(buf, '\\', '/');
        strcat(dst, buf);
    }
}
