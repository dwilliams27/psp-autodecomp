#include "gcPlayer.h"

inline void *operator new(unsigned int, void *p) { return p; }

void cStrCopy(char *, const char *);
char *cStrFormat(char *, const char *, ...);
extern void *__vec_new(void *, int, int, void (*)(void *));
extern "C" void gcReplicationGroup__gcReplicationGroup_void__0024400C(void *);

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

template <class T> T *dcast(const cBase *);

extern char *gcViewport_s_viewports;
extern int gcPlayer_s_nDefaultController;
extern unsigned short gcPlayer_s_nLastBoundController;  // 0x37D2FA
extern char *gcPlayer_s_pPlayers;                        // 0x37D87C, stride 68
extern int gcPlayer_s_nLocalControllerId[8];             // 0x37D884
extern float gcPlayer_s_fLocalControllerStrength[8];     // 0x37D8A4
extern void *D_00038890[];

extern char cBaseclassdesc[];                            // @ 0x37E6A8

struct gcPlayer_AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

struct gcPlayer_HandleEntry {
    char pad_000[0x30];
    int handle;
    char pad_034[0x33];
    signed char player;
};

static inline int gcPlayer_IsValidEntityHandle(int handle) {
    int valid;
    if (handle == 0) {
        valid = 0;
    } else {
        gcPlayer_HandleEntry *entry =
            (gcPlayer_HandleEntry *)D_00038890[handle & 0xFFFF];
        gcPlayer_HandleEntry *found = 0;
        if (entry != 0) {
            valid = found != 0;
            if (entry->handle == handle) {
                found = entry;
            }
        }
        valid = found != 0;
    }
    return valid & 0xFF;
}

static inline gcPlayer_HandleEntry *gcPlayer_GetEntityFromHandle(int handle) {
    gcPlayer_HandleEntry *entry = 0;
    if (handle != 0) {
        entry = (gcPlayer_HandleEntry *)D_00038890[handle & 0xFFFF];
    }
    return entry;
}

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
// gcPlayer::gcPlayer(cBase *)
// -----------------------------------------------------------------------------
gcPlayer::gcPlayer(cBase *parent) {
    *(cBase **)this = parent;
    *(void **)((char *)this + 4) = (void *)0x388BC8;
    *(int *)((char *)this + 8) = 0;
    *(int *)((char *)this + 0x20) = -1;
    *(int *)((char *)this + 0x24) = -1;
    *(int *)((char *)this + 0x28) = 0;
    *(int *)((char *)this + 0x2C) = 0;
    *(int *)((char *)this + 0x30) = -1;
    *(unsigned char *)((char *)this + 0x34) = 1;
    *(unsigned char *)((char *)this + 0x35) = 0;
    *(unsigned char *)((char *)this + 0x36) = 0;
    *(float *)((char *)this + 0x38) = 0.0f;
    __vec_new((char *)this + 0x3C, 1, 6,
              gcReplicationGroup__gcReplicationGroup_void__0024400C);
    cStrFormat((char *)this + 0x0C, (const char *)0x36DE48,
               ((char *)this - gcPlayer_s_pPlayers) / 68);
}

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
// gcPlayer::AssignCopy(const cBase *)  @ 0x002610a0, 240B
// -----------------------------------------------------------------------------
struct gcPlayer_half3 {
    short a;
    short b;
    short c;
};

void gcPlayer::AssignCopy(const cBase *base) {
    gcPlayer *other = dcast<gcPlayer>(base);
    int *src8 = (int *)((char *)other + 8);
    int value8 = *src8;
    int *dst8 = (int *)((char *)this + 8);
    *dst8 = value8;

    int i = 0;
    do {
        char *srcByte = (char *)(i + (int)other);
        char value = *(srcByte + 0xC);
        char *dstByte = (char *)(i + (int)this);
        i += 1;
        *(dstByte + 0xC) = value;
    } while (i < 0x14);

    *(int *)((char *)this + 0x20) = *(int *)((char *)other + 0x20);
    *(int *)((char *)this + 0x24) = *(int *)((char *)other + 0x24);
    int *src28 = (int *)((char *)other + 0x28);
    int value28 = *src28;
    int *dst28 = (int *)((char *)this + 0x28);
    int *src2c = (int *)((char *)other + 0x2C);
    int *dst2c = (int *)((char *)this + 0x2C);
    *dst28 = value28;
    int value2c = *src2c;
    *dst2c = value2c;
    *(int *)((char *)this + 0x30) = *(int *)((char *)other + 0x30);
    *(unsigned char *)((char *)this + 0x34) =
        *(unsigned char *)((char *)other + 0x34);
    *(unsigned char *)((char *)this + 0x35) =
        *(unsigned char *)((char *)other + 0x35);
    *(unsigned char *)((char *)this + 0x36) =
        *(unsigned char *)((char *)other + 0x36);
    *(float *)((char *)this + 0x38) = *(float *)((char *)other + 0x38);

    int j = 0;
    gcPlayer_half3 *dst = (gcPlayer_half3 *)((char *)this + 0x3C);
    gcPlayer_half3 *src = (gcPlayer_half3 *)((char *)other + 0x3C);
    do {
        short a = src->a;
        short b = src->b;
        short c = src->c;
        dst->a = a;
        dst->b = b;
        dst->c = c;
        j += 1;
        dst += 1;
        src += 1;
    } while (j <= 0);
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
    __asm__ volatile("" ::: "memory");
    int i = 0;
    char *viewports = gcViewport_s_viewports;
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
// gcPlayer::SetEntity(cHandleT<gcEntity>)
// -----------------------------------------------------------------------------
void gcPlayer::SetEntity(cHandleT<gcEntity> handle) {
    int current;
    if (((((current = *(int *)((char *)this + 8)) ==
           *(volatile int *)&handle)) & 0xFF) == 0) {
        if (gcPlayer_IsValidEntityHandle(current) != 0) {
            gcPlayer_GetEntityFromHandle(current)->player = -1;
        }

        int nextHandle = *(volatile int *)&handle;
        int *entityHandle = (int *)((char *)this + 8);
        *entityHandle = nextHandle;

        int next = *(int *)((char *)this + 8);
        if (gcPlayer_IsValidEntityHandle(next) != 0) {
            gcPlayer_HandleEntry *entry = gcPlayer_GetEntityFromHandle(next);
            entry->player = ((char *)this - gcPlayer_s_pPlayers) / 68;
        }
    }
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
        new (obj) gcPlayer(parent);
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
