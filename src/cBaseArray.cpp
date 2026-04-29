class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    void Write(int);
    void WriteBase(const cBase *);
};

class cReadBlock {
public:
    int _data[5];
    void ReadBase(cMemPool *, cBase *, cBase *&);
};

class cFileSystem {
public:
    static void Read(void *handle, void *buf, unsigned int size);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cBaseArray {
public:
    int *mData;
    cBase *mOwner;

    void SetSize(int);
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
};

void cBaseArray::Write(cWriteBlock &wb) const {
    int count = 0;
    if (mData != 0) {
        count = mData[-1];
    }
    wb.Write(count);

    int index = 0;
    int *tmp = mData;
    __asm__ volatile("" : "+r"(tmp));
    int offset = 0;
loop:
    {
        count = 0;
        if (mData != 0) {
            count = mData[-1];
        }
        if (index < count) {
            wb.WriteBase(*(cBase **)((char *)mData + offset));
            index += 1;
            offset += 4;
            goto loop;
        }
    }
}

void cBaseArray::Read(cReadBlock &rb) {
    int size;

    cFileSystem::Read(*(void **)rb._data[0], &size, 4);
    SetSize(size);

    cMemPool *pool = cMemPool::GetPoolFromPtr(this);
    int index;
    index = 0;
    if (index < size) {
        int offset = 0;
        do {
            rb.ReadBase(pool, mOwner, *(cBase **)((char *)mData + offset));
            index += 1;
            offset += 4;
        } while (index < size);
    }
}
