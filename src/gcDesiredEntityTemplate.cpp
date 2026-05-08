class cBase;
class cFile;
class cFileHandle;
class cMemPool;
class cObject;
class cReadBlock;
class cType;
class gcEnumeration;
class gcEnumerationEntry;

template <class T>
class cSubHandleT;

template <class T, class U>
class cHandlePairT;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void WriteBase(const cBase *);
    void End(void);
};

class cHandle {
public:
    void Read(cReadBlock &, cMemPool *);
    void Write(cWriteBlock &) const;
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

class cReadBlock {
public:
    void ReadBase(cMemPool *, cBase *, cBase *&);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class gcEntityTemplate {
public:
    int HasCategory(const cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > &) const;
    static cBase *New(cMemPool *, cBase *);
};

class gcDesiredObject {
public:
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
};

class cType {
public:
    char _pad[0x1C];
    const cType *mParent;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_0009F3F4;
extern cType *D_0009F448;
extern cType *D_0009F44C;

struct DispatchEntry {
    short offset;
    short _pad;
    cType *(*fn)(void *, short, void *);
};

class gcDesiredEntityTemplate {
public:
    cObject *Get(bool) const;
    cObject *GetObject(bool) const;
    int HasCategory(const cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > &) const;
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    gcDesiredEntityTemplate &operator=(const gcDesiredEntityTemplate &);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    const cType *GetDesiredType(void) const;
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

cObject *gcDesiredEntityTemplate::GetObject(bool b) const {
    return Get(b);
}

int gcDesiredEntityTemplate::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19") = 1;
    int rb[5];
    int inner[5];
    cBase *readBase;

    __0oKcReadBlockctR6FcFileUib(rb, file, 2, true);
    if (rb[3] != 2 || ((gcDesiredObject *)this)->Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    __0oKcReadBlockctR6FcFileUib(inner, *(cFile *)rb[0], 3, true);
    cFileSystem::Read(*(cFileHandle **)inner[0], (char *)this + 0x0C, 4);
    *(int *)((char *)this + 0x10) = 0;
    {
        cHandle *handle = (cHandle *)((char *)this + 0x10);
        handle->Read(*(cReadBlock *)inner, cMemPool::GetPoolFromPtr(handle));
    }
    __0oKcReadBlockdtv(inner, 2);

    int value = *(int *)((char *)this + 0x14);
    int tagged = value & 1;
    int flag = 0;
    if (tagged != 0) {
        flag = 1;
    }
    if (flag != 0) {
        readBase = 0;
    } else {
        readBase = (cBase *)value;
    }

    int flag2 = 0;
    if (tagged != 0) {
        flag2 = 1;
    }
    if (flag2 != 0) {
        value &= -2;
    } else {
        value = *(int *)value;
    }

    ((cReadBlock *)rb)->ReadBase(cMemPool::GetPoolFromPtr((char *)this + 0x14),
                                 (cBase *)value, readBase);
    cBase *finalBase = readBase;
    if (finalBase == 0) {
        finalBase = (cBase *)(value | 1);
    }
    *(cBase **)((char *)this + 0x14) = finalBase;
    __0oKcReadBlockdtv(rb, 2);
    return result;
}

void gcDesiredEntityTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const gcDesiredObject *)this)->Write(file);

    cWriteBlock wb2(**(cFile **)&wb._data[0], 3);
    wb2.Write(*(int *)((const char *)this + 12));
    ((const cHandle *)((const char *)this + 16))->Write(wb2);
    wb2.End();

    int val = *(int *)((const char *)this + 20);
    int flag = 0;
    if (val & 1) {
        flag = 1;
    }
    cBase *ptr;
    if (flag != 0) {
        ptr = 0;
    } else {
        ptr = (cBase *)val;
    }
    wb.WriteBase(ptr);
    wb.End();
}

const cType *gcDesiredEntityTemplate::GetType(void) const {
    if (D_0009F44C == 0) {
        if (D_0009F3F4 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F3F4 = cType::InitializeType(0, 0, 0x12C, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F44C = cType::InitializeType(0, 0, 0x1A6, D_0009F3F4,
                                           &gcDesiredEntityTemplate::New,
                                           0, 0, 0);
    }
    return D_0009F44C;
}

// ── gcDesiredEntityTemplate::GetDesiredType(void) const @ 0x001281c0 ──
const cType *gcDesiredEntityTemplate::GetDesiredType(void) const {
    if (D_0009F448 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType(
                        (const char *)0x36D894, (const char *)0x36D89C,
                        1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(
                    0, 0, 2, D_000385DC, &cNamed::New, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(
                0, 0, 3, D_000385E0, 0, 0, 0, 0);
        }
        D_0009F448 = cType::InitializeType(
            0, 0, 0x8E, D_000385E4, &gcEntityTemplate::New,
            (const char *)0x36D9B8, (const char *)0x36D9C8, 5);
    }
    return D_0009F448;
}

void gcDesiredEntityTemplate::AssignCopy(const cBase *base) {
    const gcDesiredEntityTemplate *other = 0;
    const cBase *src = base;
    gcDesiredEntityTemplate *self = this;

    if (base != 0) {
        if (D_0009F44C == 0) {
            if (D_0009F3F4 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_0009F3F4 = cType::InitializeType(0, 0, 0x12C, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_0009F44C = cType::InitializeType(0, 0, 0x1A6, D_0009F3F4,
                                               &gcDesiredEntityTemplate::New,
                                               0, 0, 0);
        }

        void *classDesc = *(void **)((char *)src + 4);
        cType *target = D_0009F44C;
        DispatchEntry *entry = (DispatchEntry *)((char *)classDesc + 8);
        short offset = entry->offset;
        cType *(*fn)(void *, short, void *) = entry->fn;
        cType *type = fn((char *)src + offset, offset, fn);
        int isValid;

        if (target != 0) {
            goto have_target;
        }
        isValid = 0;
        goto cast_done;

have_target:
        if (type != 0) {
loop_cast:
            if (type == target) {
                isValid = 1;
            } else {
                type = (cType *)type->mParent;
                if (type != 0) {
                    goto loop_cast;
                }
                goto invalid_cast;
            }
        } else {
invalid_cast:
            isValid = 0;
        }

cast_done:
        if (isValid != 0) {
            other = (const gcDesiredEntityTemplate *)src;
        }
    }
    self->operator=(*other);
}

int gcDesiredEntityTemplate::HasCategory(
    const cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > &category) const {
    gcEntityTemplate *entityTemplate = (gcEntityTemplate *)Get(1);
    if (entityTemplate != 0) {
        return entityTemplate->HasCategory(category);
    }
    return 0;
}
