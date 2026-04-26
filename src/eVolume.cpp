// Mixed small functions routed into this TU by the orchestrator.

class cBase;

template <class T> T *dcast(const cBase *);

// ============================================================
// eVolume::UpdateLocalToWorld(void) @ 0x0005D928
// ============================================================

class eWorld;

class eVolume {
public:
    char _pad0[0x20];
    eWorld *mWorld;

    void UpdateLocalToWorld(void);
};

class eWorld {
public:
    void UpdateVolumeLocation(eVolume *);
};

#pragma control sched=1
void eVolume::UpdateLocalToWorld(void) {
    eWorld *world = mWorld;
    if (world != 0) {
        world->UpdateVolumeLocation(this);
    }
}
#pragma control sched=2

// ============================================================
// simple vtable-setting constructors
// ============================================================

void gcEntityGeomConfig_gcEntityGeomConfig(void *self, cBase *parent);
void gcEntityControllerTemplate_gcEntityControllerTemplate(void *self, cBase *parent);

class gcDynamicModelConfig {
public:
    gcDynamicModelConfig(cBase *);
};

gcDynamicModelConfig::gcDynamicModelConfig(cBase *parent) {
    gcEntityGeomConfig_gcEntityGeomConfig(this, parent);
    *(void **)((char *)this + 4) = (void *)0x38C5E0;
}

class gcParticleSystemConfig {
public:
    gcParticleSystemConfig(cBase *);
};

gcParticleSystemConfig::gcParticleSystemConfig(cBase *parent) {
    gcEntityGeomConfig_gcEntityGeomConfig(this, parent);
    *(void **)((char *)this + 4) = (void *)0x38CDE8;
}

class gcStationaryControllerTemplate {
public:
    gcStationaryControllerTemplate(cBase *);
};

gcStationaryControllerTemplate::gcStationaryControllerTemplate(cBase *parent) {
    gcEntityControllerTemplate_gcEntityControllerTemplate(this, parent);
    *(void **)((char *)this + 4) = (void *)0x38D330;
}

// ============================================================
// gcValEntityIsActive::AssignCopy(const cBase *) @ 0x003323C4
// ============================================================

class gcDesiredEntity {
public:
    gcDesiredEntity &operator=(const gcDesiredEntity &);
};

class gcValEntityIsActive {
public:
    void AssignCopy(const cBase *);
};

void gcValEntityIsActive::AssignCopy(const cBase *base) {
    gcValEntityIsActive *other = dcast<gcValEntityIsActive>(base);
    gcDesiredEntity &srcDesired = *(gcDesiredEntity *)((char *)other + 8);
    ((gcDesiredEntity *)((char *)this + 8))->operator=(srcDesired);
}
