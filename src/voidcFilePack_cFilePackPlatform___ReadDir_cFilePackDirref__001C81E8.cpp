// cFilePack<cFilePackPlatform>::ReadDir(cFilePackDir &) @ 0x001c81e8

struct cFilePackDir {
    int   unk0;          // 0x00
    int   unk4;          // 0x04
    int   mFileCount;    // 0x08  (count of file entries, *8)
    int   mFiles;        // 0x0C  (assigned running ptr)
    int   mDirCount;     // 0x10  (count of sub-dirs, *0x18)
    cFilePackDir *mDirs; // 0x14  (assigned running ptr)
};

class cFilePackPlatform;

template <class T>
class cFilePack {
public:
    int   unk0;   // 0x00
    int   mCur;   // 0x04  running buffer offset/pointer
    int   unk8;   // 0x08
    int   unkC;   // 0x0C
    int   mCur2;  // 0x10  second running offset
    int   mDirNo; // 0x14  directory counter

    void ReadDir(cFilePackDir &dir);
};

template <class T>
void cFilePack<T>::ReadDir(cFilePackDir &dir)
{
    int filesPtr = 0;
    int fileCount = dir.mFileCount;
    if (fileCount != 0) {
        filesPtr = mCur;
    }
    dir.mFiles = filesPtr;
    mCur = mCur + fileCount * 8;

    mCur2 = mCur2 + dir.mFileCount;

    int dirsPtr = 0;
    int dirCount = dir.mDirCount;
    if (dirCount != 0) {
        dirsPtr = mCur;
    }
    dir.mDirs = (cFilePackDir *)dirsPtr;
    mCur = mCur + dirCount * 0x18;

    for (unsigned int i = 0; i < (unsigned int)dir.mDirCount; i++) {
        ReadDir(dir.mDirs[i]);
    }

    mDirNo = mDirNo + 1;
}

template class cFilePack<cFilePackPlatform>;
