class cBase;
class cFile;
class cType;

extern "C" void free(void *);

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void End(void);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

struct cObject {
    char _pad0[0x34];
    cObject *prev;
    cObject *next;
};

struct cGroupNode {
    char _pad0[0x28];
    unsigned short flags;
    char _pad2[6];
    cHandle handle;
    cGroupNode *next;
};

class cGroup {
public:
    char _pad[0xC];
    cGroupNode *mList;

    ~cGroup();
    const cType *GetType(void) const;
    void Write(cFile &) const;
    void OnObjectDeleted(cObject *);
    void RemoveAll(void);
    void ClearVisitedReferences(unsigned int);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        if (pool != 0) {
            char *block = ((char **)pool)[9];
            DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
            short off = rec->offset;
            void (*fn)(void *, void *) = rec->fn;
            fn(block + off, p);
        } else {
            free(p);
        }
    }
};

class cListSubscriber {
public:
    const cType *GetType(void) const;
};

extern cType *D_000385DC;
extern cType *D_00038880;
extern cType *D_00040C94;

cGroup::~cGroup() {
    *(void **)((char *)this + 4) = (void *)0x37EA80;
    RemoveAll();
    *(void **)((char *)this + 4) = (void *)0x37E6A8;
}

const cType *cGroup::GetType(void) const {
    if (D_00040C94 == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType((const char *)0x36C830,
                                               (const char *)0x36C838,
                                               1, 0, 0, 0, 0, 0);
        }
        D_00040C94 = cType::InitializeType(0, 0, 4, D_000385DC,
                                           0, 0, 0, 0);
    }
    return D_00040C94;
}

void cGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    cGroupNode *node = mList;
    if (node != 0) {
        do {
            int write = node->flags & 4;
            write = write != 0;
            write = (unsigned char)write;
            if (write != 0) {
                wb.Write(true);
                node->handle.Write(wb);
            }
            cGroupNode *nextNode = 0;
            cGroupNode *next = node->next;
            if (next != mList) {
                nextNode = next;
            }
            node = nextNode;
        } while (node != 0);
    }
    wb.Write(false);
    wb.End();
}

void cGroup::OnObjectDeleted(cObject *object) {
    if (object != 0 && object->next != 0) {
        cObject *prev = object->prev;
        if (prev != 0) {
            if ((cObject *)mList != object) {
                goto notHead;
            }
            mList = (cGroupNode *)prev;
            prev = object->prev;
notHead:
            object->next->prev = prev;
            prev->next = object->next;
            object->next = 0;
            object->prev = 0;
            if ((cObject *)mList == object) {
                mList = 0;
            }
        }
    }
}

void cGroup::ClearVisitedReferences(unsigned int flags) {
    cGroupNode *node = mList;
    if (node != 0) {
        unsigned int mask = ~flags | 0xFFFF01FF;
        do {
            cGroupNode *nextNode = 0;
            cGroupNode *next;
            node->flags = node->flags & mask;
            next = node->next;
            if (next != mList) {
                nextNode = next;
            }
            node = nextNode;
        } while (node != 0);
    }
}

const cType *cListSubscriber::GetType(void) const {
    if (D_00038880 == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType((const char *)0x36C830,
                                               (const char *)0x36C838,
                                               1, 0, 0, 0, 0, 0);
        }
        D_00038880 = cType::InitializeType(0, 0, 0x187, D_000385DC,
                                           0, 0, 0, 0);
    }
    return D_00038880;
}
