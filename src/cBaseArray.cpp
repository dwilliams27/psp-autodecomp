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
    void Set(int, cBase *);
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
    void Reset(cMemPool *);
};

struct cBaseArray_CloneEntry {
    short offset;
    short _pad;
    cBase *(*func)(void *, cMemPool *, cBase *);
};

typedef void (*cBaseArray_SetFn)(void *, int, void *, short);

struct cBaseArray_SetEntry {
    short offset;
    short _pad;
    cBaseArray_SetFn func;
};

struct cBaseArray_ResetEntry {
    short offset;
    short _pad;
    void (*func)(void *, cMemPool *, int);
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

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0fKcBaseArrayDSetiP6FcBase, 0x98\n");

void cBaseArray::Set(int index, cBase *base) {
    int offset = index * 4;
    cBase **slot = (cBase **)((char *)mData + offset);
    cBase *old = *slot;

    if (old != base) {
        if (old != 0) {
            char *type = *(char **)((char *)old + 4);
            cBaseArray_SetEntry *entry = (cBaseArray_SetEntry *)(type + 0x50);
            cBaseArray_SetFn fn = entry->func;
            short adjust = entry->offset;
            fn((char *)old + adjust, 3, (void *)fn, adjust);
            *(cBase **)((char *)mData + offset) = 0;
            slot = (cBase **)((char *)mData + offset);
        }
        *slot = base;
        if (base != 0) {
            *(cBase **)((char *)base + 0) = mOwner;
        }
    }
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

void cBaseArray::Reset(cMemPool *pool) {
    int count = 0;
    if (mData != 0) {
        count = mData[-1];
    }

    int index = 0;
    if (index < count) {
        int offset = 0;
        do {
            cBase *item = *(cBase **)((char *)mData + offset);
            if (item != 0) {
                char *type = *(char **)((char *)item + 4);
                cBaseArray_ResetEntry *entry =
                    (cBaseArray_ResetEntry *)(type + 0x38);
                entry->func((char *)item + entry->offset, pool, 1);
            }
            index += 1;
            offset += 4;
        } while (index < count);
    }
}
