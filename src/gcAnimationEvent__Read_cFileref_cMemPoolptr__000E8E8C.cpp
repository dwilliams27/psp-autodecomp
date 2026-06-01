// gcAnimationEvent::Read(cFile &, cMemPool *) @ 0x000e8e8c
// Isolated split-TU build of a single method symbol:
//   __0fQgcAnimationEventEReadR6FcFileP6IcMemPool

class cBase;
class cFile;
class cFileHandle;
class cMemPool;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

struct EventReadSlot {
    short offset;
    short pad;
    void (*fn)(void *, cFile *, cMemPool *);
};

class gcAnimationEvent {
public:
    cBase *m_parent;          // 0x00
    void *m_vtable;           // 0x04
    int mNameIndex;           // 0x08
    char _pad0c[4];           // 0x0C  (gcEvent subobject base)
    void *mField10;           // 0x10  (event vtable ptr)

    int Read(cFile &, cMemPool *);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

int gcAnimationEvent::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];

    __0oKcReadBlockctR6FcFileUib(rb, file, 1, true);
    if (rb[3] != 1) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 8, 4);

    void *vtbl = this->mField10;
    char *event = (char *)this + 0x0C;
    EventReadSlot *slot = (EventReadSlot *)((char *)vtbl + 0x30);
    short offset = slot->offset;
    cFile *fh = (cFile *)rb[0];
    char *target = event + offset;
    cMemPool *p = cMemPool::GetPoolFromPtr(event);
    slot->fn(target, fh, p);

    __0oKcReadBlockdtv(rb, 2);
    return result;
}
