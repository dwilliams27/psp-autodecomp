#ifndef GCPLAYER_H
#define GCPLAYER_H

class cFile;
class gcCamera;
class cBase;
class cMemPool;
class cType;

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
    static void HandleClientWarp(void);
    static void ResetController(int);
    static void ResetControllers(void);
    static void ResetOtherControllers(void);
    static int BindDefaultController(void);
    static int AssignNew(void);
    static int GetStartingController(void);
    static int BindLocalController(void);
    static void AssignLocalController(int, bool);
    static gcPlayer *GetPlayerForCamera(const gcCamera *);
    static cBase *New(cMemPool *, cBase *);
    void *GetCamera(void) const;
    void GetName(char *) const;
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
