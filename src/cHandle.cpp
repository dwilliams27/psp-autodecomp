// cHandle — opaque handle into the global cManager object table.
//
// Functions matched here:
//   cHandle::Write(cOutStream &) const  @ 0x000092f0  128B
//   cHandle::Read(cInStream &)          @ 0x00009370  124B

class cInStream;
class cOutStream;
class cWriteBlock;

class cObjectKey {
public:
    unsigned int mKey;
    void Read(cInStream &);
    void Write(cOutStream &) const;
};

class cObject {
public:
    char _pad0[0x28];
    unsigned short mFlags;   // 0x28
    char _pad2a[6];
    int mIndex;              // 0x30
};

class cObjectFull {
public:
    char _pad0[0x20];
    unsigned int mWord20;    // 0x20
    unsigned int mWord24;    // 0x24
    char _pad28[8];
    int mIndex;              // 0x30
};

struct cGUID {
    int a;
    int b;
};

class cFile {
public:
    char _pad0[5];
    bool mField5;
    bool mField6;

    bool AddDependency(const cObject *);
};

class cWriteBlock {
public:
    cFile *mFile;
    unsigned int mOffset;

    void Write(bool);
    void Write(short);
    void Write(unsigned int);
    void Write(int);
    void Write(const cGUID &);
};

class cName {
public:
    void Write(cWriteBlock &) const;
};

class cType {
public:
    unsigned int mFlags;
    unsigned int mTypeId;
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct TypeDispatchEntry {
    short offset;
    short _pad;
    cType *(*fn)(void *);
};

struct WriteDispatchEntry {
    short offset;
    short _pad;
    void (*fn)(void *, cFile *);
};

class cManager {
public:
    static cObject *Find(const cObjectKey &);
};

class cHandle : public cObjectKey {
public:
    void Read(cInStream &);
    void Write(cOutStream &) const;
    void Write(cWriteBlock &) const;
};

extern void *D_00038890[];   // global handle-pointer table

void cHandle::Write(cWriteBlock &wb) const {
    int value = mKey;
    void *resolved;
    if (value == 0) {
        resolved = 0;
    } else {
        void *entry = D_00038890[value & 0xFFFF];
        resolved = 0;
        if (entry != 0) {
            if (*(int *)((char *)entry + 0x30) == value) {
                resolved = entry;
            }
        }
    }

    void *object = resolved;
    if (object != 0) {
        char *typeInfo = *(char **)((char *)object + 4);
        TypeDispatchEntry *typeEntry = (TypeDispatchEntry *)(typeInfo + 8);
        cType *type = typeEntry->fn((char *)resolved + typeEntry->offset);
        wb.Write((short)type->mTypeId);
        wb.Write((short)1);
        wb.Write(*(const cGUID *)((char *)object + 0x20));
        wb.Write(*(unsigned int *)((char *)object + 0x2C));
        if (wb.mFile->mField5 == 0) {
            ((const cName *)((char *)object + 8))->Write(wb);
        }

        cMemPool *objectPool = cMemPool::GetPoolFromPtr(object);
        cMemPool *thisPool = cMemPool::GetPoolFromPtr(this);
        register int added asm("a1") =
            wb.mFile->AddDependency((const cObject *)object);
        register cFile *file asm("a0") = wb.mFile;
        register int writeObject asm("a2") = 0;
        register int field5 asm("t0") = file->mField5;
        register unsigned int flags asm("a3") =
            *(unsigned short *)((char *)object + 0x28);
        if (field5 != 0) {
            goto setWriteObject;
        } else {
            int localType = 1;
            if (type->mFlags & 1) {
                localType = 0;
            }
            if (localType == 0) {
                goto afterWriteObject;
            }
        }
setWriteObject:
        writeObject = 1;
afterWriteObject:

        int flagged = 0;
        int samePool = objectPool == thisPool;
        if ((flags & 0x80) != 0) {
            flagged = 1;
        } else if ((flags & 0x100) != 0) {
            flagged = 1;
        }
        flagged &= 0xFF;

        int externalPool = 0;
        if (file->mField5 != 0) {
            if (samePool == 0) {
                int poolFlag = 0;
                if (*(int *)(*(char **)(objectPool + 0x28) + 0x44) & 1) {
                    poolFlag = 1;
                    __asm__ volatile("" ::: "memory");
                }
                if ((poolFlag & 0xFF) == 0) {
                    externalPool = 1;
                }
            }
        }
        externalPool &= 0xFF;

        if ((writeObject & 0xFF) != 0) {
            if (added == 0) {
                goto writeFalse;
            }
            if (externalPool != 0) {
                goto writeFalse;
            }
            if ((flagged & 0xFF) != 0) {
                if (file->mField6 == 0) {
                    goto writeFalse;
                }
            }

            bool writeContents;
            if (samePool == 0) {
                int typeFlag = 0;
                if (type->mFlags & 1) {
                    typeFlag = 1;
                }
                writeContents = false;
                if (typeFlag != 0) {
                    writeContents = true;
                }
            } else {
                writeContents = true;
            }
            wb.Write(writeContents);
            if (writeContents != 0) {
                char *writeTypeInfo = *(char **)((char *)object + 4);
                WriteDispatchEntry *writeEntry =
                    (WriteDispatchEntry *)(writeTypeInfo + 0x28);
                writeEntry->fn((char *)resolved + writeEntry->offset, wb.mFile);
            }
        } else {
            wb.Write(true);
        }
    } else {
        wb.Write(-1);
    }
    return;

writeFalse:
    wb.Write(false);
}

// 0x000092f0 — encrypt-and-write the handle's key to a cOutStream.
void cHandle::Write(cOutStream &s) const {
    cObjectKey out;
    unsigned int idx = mKey;
    void *obj;
    if (idx == 0) {
        obj = 0;
    } else {
        void *cand = *(D_00038890 + (idx & 0xFFFF));
        obj = 0;
        if (cand != 0 && *(int *)((char *)cand + 0x30) == (int)idx) {
            obj = cand;
        }
    }
    unsigned int key = 0;
    if (obj != 0) {
        key = ((cObjectFull *)obj)->mWord20 ^ ((cObjectFull *)obj)->mWord24;
    }
    out.mKey = key;
    out.Write(s);
}

// 0x00009370 — read a key from cInStream, look up via cManager::Find,
// and store the resolved index back into the handle's mKey.
void cHandle::Read(cInStream &s) {
    cObjectKey result;
    cObjectKey input;
    input.mKey = 0;
    input.Read(s);
    cObject *obj = cManager::Find(input);
    if (obj != 0) {
        if (!(((obj->mFlags & 0x10) != 0) & 0xFF)) {
            obj = 0;
        }
    }
    int idx = 0;
    if (obj != 0) {
        idx = obj->mIndex;
    }
    result.mKey = idx;
    *(cObjectKey *)this = result;
}
