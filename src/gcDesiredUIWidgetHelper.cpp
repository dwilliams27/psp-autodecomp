// gcDesiredUIWidgetHelper — gcAll_psp.obj
//   0x0012e844  Write(cWriteBlock &) const                 (92B)
//   0x0012e8a0  Read(cReadBlock &)                         (256B)

class cFile;
class cFileHandle;
class cMemPool;

class cWriteBlock {
public:
    cFile *_file;
    int _pos;
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(unsigned int);
    void End(void);
};

class cReadBlock {
public:
    cFile *_file;
    int _pos;
    int _data2;
    int _data3;
    int _data4;
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cHandle {
public:
    int mId;
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &, cMemPool *);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct cFile_Layout {
    cFileHandle *_handle;
};

extern void *D_00038890[];

struct gcDesiredUIWidgetHelper {
    int mField0;
    cHandle mField4;
    unsigned int mField8;
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
};

// 0x0012e844 — Write(cWriteBlock &) const, 92B
void gcDesiredUIWidgetHelper::Write(cWriteBlock &outer) const {
    cWriteBlock wb(*outer._file, 1);
    wb.Write(mField0);
    mField4.Write(wb);
    wb.Write(mField8);
    wb.End();
}

// 0x0012e8a0 — Read(cReadBlock &), 256B
void gcDesiredUIWidgetHelper::Read(cReadBlock &outer) {
    cReadBlock rb(*outer._file, 1, true);
    cFileSystem::Read(((cFile_Layout *)rb._file)->_handle, this, 4);
    mField4.mId = 0;
    cMemPool *pool = cMemPool::GetPoolFromPtr(&mField4);
    mField4.Read(rb, pool);
    cFileSystem::Read(((cFile_Layout *)rb._file)->_handle, &mField8, 4);

    int h = mField4.mId;
    void *t;
    if (h == 0) {
        t = 0;
    } else {
        void *e = D_00038890[h & 0xFFFF];
        t = 0;
        if (e != 0 && *(int *)((char *)e + 0x30) == h) {
            t = e;
        }
    }

    if (t != 0) {
        int dead;
        __asm__ volatile("andi %0, $0, 0xff" : "=r"(dead));
        if (dead) {
            mField8 = (mField8 & 0x8000FFFFu) | 0x10000u;
        }
    }
}
