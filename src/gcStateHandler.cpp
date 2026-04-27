// gcStateHandler — derived from gcStateHandlerBase. Methods here are the
// derived-class entry points that delegate to the base implementation.

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class gcStateHandlerBase {
public:
    gcStateHandlerBase(cBase *);
    ~gcStateHandlerBase();
    gcStateHandlerBase &operator=(const gcStateHandlerBase &);
    void Write(cFile &) const;
};

class gcStateHandler;
gcStateHandler *dcast(const cBase *);

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cMemPool_local {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

extern "C" void *cMemPool_GetPoolFromPtr(const void *);

class gcStateHandler : public gcStateHandlerBase {
public:
    gcStateHandler(cBase *);
    ~gcStateHandler();
    static void operator delete(void *p) {
        void *pool = cMemPool_GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        rec->fn(block + rec->offset, p);
    }
    void AssignCopy(const cBase *other);
    void Write(cFile &file) const;
};

extern char gcStateHandlervirtualtable[];

// 0x00258210, 52B
void gcStateHandler::AssignCopy(const cBase *other) {
    gcStateHandler *src = dcast(other);
    ((gcStateHandlerBase *)this)->operator=(*(gcStateHandlerBase *)src);
}

// 0x0010a930, 76B
void gcStateHandler::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcStateHandlerBase *)this)->Write(file);
    wb.End();
}

// 0x002583b0, 124B
gcStateHandler::~gcStateHandler() {
    *(void **)((char *)this + 4) = gcStateHandlervirtualtable;
}
