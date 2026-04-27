class cBase;

class gcProfile {
public:
    unsigned char m_pad[0x23C];
    unsigned int m_crc;

    gcProfile(cBase *);
    unsigned int CalcCRC(void) const;
    bool IsDirty(void) const;
};

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
