#include "gcPlayer.h"

void cStrCopy(char *, const char *);

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

extern char *gcViewport_s_viewports;
extern int gcPlayer_s_nDefaultController;
extern unsigned short gcPlayer_s_nLastBoundController;  // 0x37D2FA
extern char *gcPlayer_s_pPlayers;                        // 0x37D87C, stride 68
extern int gcPlayer_s_nLocalControllerId[8];             // 0x37D884
extern float gcPlayer_s_fLocalControllerStrength[8];     // 0x37D8A4

extern char cBaseclassdesc[];                            // @ 0x37E6A8

extern "C" void gcPlayer__gcPlayer_cBaseptr(void *self, cBase *parent);

struct gcPlayer_AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

extern const char gcPlayer_type_name[];                  // @ 0x36D894
extern const char gcPlayer_type_desc[];                  // @ 0x36D89C

// -----------------------------------------------------------------------------
// gcPlayer::GetCamera(void) const
// -----------------------------------------------------------------------------
void *gcPlayer::GetCamera(void) const {
    int idx = *(int *)((char *)this + 48);
    int result = 0;
    if (idx >= 0) {
        char *base = gcViewport_s_viewports;
        int entry = 0;
        if (base != 0) {
            entry = (int)base + idx * 5008;
        }
        result = entry + 0x10;
    }
    return (void *)result;
}

// -----------------------------------------------------------------------------
// gcPlayer::GetName(char *) const
// -----------------------------------------------------------------------------
void gcPlayer::GetName(char *dest) const {
    cStrCopy(dest, (const char *)this + 12);
}

// -----------------------------------------------------------------------------
// gcPlayer::Write(cFile &) const
// -----------------------------------------------------------------------------
void gcPlayer::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    wb.End();
}

// -----------------------------------------------------------------------------
// gcPlayer::~gcPlayer(void)  @ 0x0011ee4c, 100B
//
// Canonical C++ destructor. SNC's ABI auto-emits the (this != 0) guard and the
// deleting-tail dispatch via operator delete on (flag & 1). Body just resets
// the classdesc pointer at offset 4 to the parent (cBase) classdesc.
// -----------------------------------------------------------------------------
gcPlayer::~gcPlayer() {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}

// -----------------------------------------------------------------------------
// gcPlayer::GetPlayerForCamera(const gcCamera *) static
// -----------------------------------------------------------------------------
gcPlayer *gcPlayer::GetPlayerForCamera(const gcCamera *cam) {
    gcPlayer *p = (gcPlayer *)gcPlayer_s_pPlayers;
    char *viewports = gcViewport_s_viewports;
    int i = 0;
    do {
        int vpIdx = *(int *)((char *)p + 48);
        if (vpIdx >= 0) {
            char *vp = 0;
            if (viewports != 0) {
                vp = viewports + vpIdx * 5008;
            }
            if ((const gcCamera *)(vp + 16) == cam) {
                return p;
            }
        }
        i++;
        p = (gcPlayer *)((char *)p + 68);
    } while (i < 8);
    return 0;
}

// -----------------------------------------------------------------------------
// gcPlayer::BindDefaultController(void) static
// -----------------------------------------------------------------------------
int gcPlayer::BindDefaultController(void) {
    int id = GetStartingController();
    if (id < 0) return -1;
    int s = id & 0xffff;
    gcPlayer_s_nDefaultController = id;
    s = s & 0xffff;
    gcPlayer_s_nLastBoundController = (unsigned short)s;
    return id;
}

// -----------------------------------------------------------------------------
// gcPlayer::AssignNew(void) static
// -----------------------------------------------------------------------------
int gcPlayer::AssignNew(void) {
    int id = BindLocalController();
    if (id < 0) return -1;
    AssignLocalController(id, true);
    return id;
}

// -----------------------------------------------------------------------------
// gcPlayer::ResetController(int) static
// -----------------------------------------------------------------------------
void gcPlayer::ResetController(int id) {
    int *p = (int *)(gcPlayer_s_pPlayers + id * 68);
    p[8] = -1;
    gcPlayer_s_nLocalControllerId[id] = -1;
    gcPlayer_s_fLocalControllerStrength[id] = 0.0f;
}

// -----------------------------------------------------------------------------
// gcPlayer::ResetOtherControllers(void) static
// -----------------------------------------------------------------------------
void gcPlayer::ResetOtherControllers(void) {
    for (int i = 1; i < 8; i++) {
        ResetController(i);
    }
}

// -----------------------------------------------------------------------------
// gcPlayer::ResetControllers(void) static
// -----------------------------------------------------------------------------
void gcPlayer::ResetControllers(void) {
    for (int i = 0; i < 8; i++) {
        ResetController(i);
    }
    gcPlayer_s_nDefaultController = -1;
}

// -----------------------------------------------------------------------------
// gcPlayer::New(cMemPool *, cBase *) static  @ 0x00261190, 124B
// -----------------------------------------------------------------------------
cBase *gcPlayer::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    gcPlayer_AllocRec *rec = (gcPlayer_AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcPlayer *result = 0;
    gcPlayer *obj = (gcPlayer *)rec->fn(base, 0x44, 4, 0, 0);
    if (obj != 0) {
        gcPlayer__gcPlayer_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// -----------------------------------------------------------------------------
// gcPlayer::GetType(void) const  @ 0x0026120c, 160B
// -----------------------------------------------------------------------------
static cType *type_base;
static cType *type_gcPlayer;

const cType *gcPlayer::GetType(void) const {
    if (!type_gcPlayer) {
        if (!type_base) {
            type_base = cType::InitializeType(gcPlayer_type_name, gcPlayer_type_desc, 1,
                                              0, 0, 0, 0, 0);
        }
        type_gcPlayer = cType::InitializeType(0, 0, 0xBE, type_base, &gcPlayer::New,
                                              0, 0, 0);
    }
    return type_gcPlayer;
}
