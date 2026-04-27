class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

void cFile_SetCurrentPos(void *, unsigned int);

class cNamed {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

struct gcProfile_AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

extern "C" void gcProfile__gcProfile_cBaseptr(void *self, cBase *parent);

class gcProfile {
public:
    unsigned char m_pad[0x23C];
    unsigned int m_crc;

    gcProfile(cBase *);
    unsigned int CalcCRC(void) const;
    bool IsDirty(void) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    static cBase *New(cMemPool *, cBase *);
};

// ── gcProfile::Write(cFile &) const @ 0x000FECDC ──
void gcProfile::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const cNamed *)this)->Write(file);
    wb.End();
}

// ── gcProfile::Read(cFile &, cMemPool *) @ 0x000FED28 ──
int gcProfile::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && ((cNamed *)this)->Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── gcProfile::New(cMemPool *, cBase *) static @ 0x0024A848 ──
cBase *gcProfile::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    gcProfile_AllocRec *rec = (gcProfile_AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcProfile *result = 0;
    gcProfile *obj = (gcProfile *)rec->fn(base, 0x240, 4, 0, 0);
    if (obj != 0) {
        gcProfile__gcProfile_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

bool gcProfile::IsDirty(void) const {
    unsigned int crc = m_crc;
    return crc != CalcCRC();
}

// ── gcProfile::gcProfile(cBase *) @ 0x000FEDE4 ──
gcProfile::gcProfile(cBase *parent) {
    *(cBase **)((char *)this + 0) = parent;
    *(short *)((char *)this + 0x1C) = 0;
    *(short *)((char *)this + 0x1E) = 0;
    *(unsigned char *)((char *)this + 0x8) = 0;
    *(void **)((char *)this + 0x4) = (void *)0x387F98;
    *(int *)((char *)this + 0x220) = 0;
    *(int *)((char *)this + 0x224) = 0;
    *(int *)((char *)this + 0x228) = 0;
    *(int *)((char *)this + 0x22C) = 0;
    *(int *)((char *)this + 0x23C) = 0;
    *(short *)((char *)this + 0x20) = 0;
    *(short *)((char *)this + 0x120) = 0;
    *(unsigned char *)((char *)this + 0x230) = 0;
}

extern "C" unsigned long _strtoul_r(void *, const char *, char **, int);

extern "C" unsigned long strtoul(const char *nptr, char **endptr, int base) {
    return _strtoul_r(*(void **)0x37DC6C, nptr, endptr, base);
}

class ePhysicsControllerConfig {
public:
    ePhysicsControllerConfig(cBase *);
};

class eKeyframedControllerConfig : public ePhysicsControllerConfig {
public:
    eKeyframedControllerConfig(cBase *);
};

#pragma control sched=1
eKeyframedControllerConfig::eKeyframedControllerConfig(cBase *parent)
    : ePhysicsControllerConfig(parent) {
    *(void **)((char *)this + 4) = (void *)0x383DA8;
}
#pragma control sched=2

class eVirtualTexture {
public:
    eVirtualTexture(cBase *);
};

class eShadowVolumeTexture : public eVirtualTexture {
public:
    eShadowVolumeTexture(cBase *);
};

#pragma control sched=1
eShadowVolumeTexture::eShadowVolumeTexture(cBase *parent)
    : eVirtualTexture(parent) {
    *(void **)((char *)this + 4) = (void *)0x385AA0;
}
#pragma control sched=2

class gcEntityController {
public:
    gcEntityController(cBase *);
};

class gcCreatureController : public gcEntityController {
public:
    gcCreatureController(cBase *);
};

gcCreatureController::gcCreatureController(cBase *parent)
    : gcEntityController(parent) {
    *(void **)((char *)this + 4) = (void *)0x38B790;
}
