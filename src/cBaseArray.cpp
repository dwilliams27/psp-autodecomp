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

    cBaseArray &operator=(const cBaseArray &);
    void RemoveAll(void);
    int *Allocate(int);
    void SetSize(int);
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
};

struct cBaseArray_CloneEntry {
    short offset;
    short _pad;
    cBase *(*func)(void *, cMemPool *, cBase *);
};

cBaseArray &cBaseArray::operator=(const cBaseArray &other) {
    RemoveAll();

    int count = 0;
    if (other.mData != 0) {
        count = other.mData[-1];
    }

    cMemPool *pool = cMemPool::GetPoolFromPtr(this);
    mData = Allocate(count);

    int index = 0;
    if (index < count) {
        int offset = 0;
        do {
            void *copy = 0;
            void *src = *(void **)((char *)other.mData + offset);
            if (src != 0) {
                char *dispatch = *(char **)((char *)src + 4);
                cBaseArray_CloneEntry *clone =
                    (cBaseArray_CloneEntry *)(dispatch + 0x10);
                copy = clone->func((char *)src + clone->offset, pool, mOwner);
            }
            *(void **)((char *)mData + offset) = copy;
            index += 1;
            offset += 4;
        } while (index < count);
    }

    return *this;
}

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
