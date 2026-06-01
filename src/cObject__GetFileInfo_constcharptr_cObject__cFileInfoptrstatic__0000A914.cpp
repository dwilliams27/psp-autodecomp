// Split TU for cObject::GetFileInfo(const char *, cObject::cFileInfo *) static.
// Keep local declarations minimal so this match does not perturb the
// existing cObject.cpp translation unit.

class cFile {
public:
    char _data[272];
    cFile(void);
    bool Open(const char *filename);
    void Close(bool flag);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cObject {
public:
    struct cFileInfo {
        int mField0;
        int mField4;
        char mName[0x18];
        int mField20;
        int mField24;
        int mField28;
    };
    static void ReadHeader(cFile &file, cFileInfo *info);
    static int GetFileInfo(const char *filename, cFileInfo *info);
};

int cObject::GetFileInfo(const char *filename, cFileInfo *info) {
    cFile file;
    int result;

    if (!file.Open(filename)) {
        file.Close(true);
        return 0;
    }

    result = 1;
    __asm__ volatile("" ::: "memory");
    {
        cReadBlock rb(file, 1, false);
        cObject::ReadHeader(file, info);
        file.Close(false);
    }
    file.Close(true);

    return result;
}
