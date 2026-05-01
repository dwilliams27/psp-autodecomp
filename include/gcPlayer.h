#ifndef GCPLAYER_H
#define GCPLAYER_H

class cFile;
class gcCamera;
class gcEntity;
class cBase;
class cMemPool;
class cType;
class nwConnectionHandle;
class gcReplicationVisitor;

template <class T>
class cHandleT {
public:
    int mIndex;
};

namespace gcPlayer_priv {
    struct DeleteRecord {
        short offset;
        short pad;
        void (*fn)(void *, void *);
    };
    class cMemPoolNS {
    public:
        static cMemPoolNS *GetPoolFromPtr(const void *);
    };
}

class gcPlayer {
public:
    gcPlayer(cBase *);
    static void HandleClientWarp(void);
    static void Reset(void);
    static void ResetController(int);
    static void ResetControllers(void);
    static void ResetOtherControllers(void);
    static int BindDefaultController(void);
    static int AssignNew(void);
    static void AssignController(int, int);
    static int GetStartingController(void);
    static int BindLocalController(void);
    static void AssignLocalController(int, bool);
    static void AssignLocalControllers(bool);
    static gcPlayer *GetPlayerForConnection(nwConnectionHandle);
    static gcPlayer *GetPlayerForCamera(const gcCamera *);
    static int FindAvailablePlayer(void);
    static void MemCardReplicate(gcReplicationVisitor &);
    static cBase *New(cMemPool *, cBase *);
    void SetEntity(cHandleT<gcEntity>);
    void *GetCamera(void) const;
    void GetName(char *) const;
    void AssignController(int, bool);
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    ~gcPlayer();

    // Inline so SNC inlines it into the deleting-destructor variant.
    static void operator delete(void *p) {
        gcPlayer_priv::cMemPoolNS *pool =
            gcPlayer_priv::cMemPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        gcPlayer_priv::DeleteRecord *rec =
            (gcPlayer_priv::DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

#endif
