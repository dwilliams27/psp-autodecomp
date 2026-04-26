class cBase;

class gcProfile {
public:
    unsigned char m_pad[0x23C];
    unsigned int m_crc;

    unsigned int CalcCRC(void) const;
    bool IsDirty(void) const;
};

bool gcProfile::IsDirty(void) const {
    unsigned int crc = m_crc;
    return crc != CalcCRC();
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
