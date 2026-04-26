// Mixed small functions routed into this TU by the orchestrator.
// Functions:
//   0x0005f788 eStaticSpotLight::eStaticSpotLight(cBase *)        eAll_psp.obj
//   0x000d4d4c gcNamedSet::Write(cFile &) const                   gcAll_psp.obj
//   0x00155eb0 gcRigidBodyControllerTemplate::gcRigidBodyControllerTemplate(cBase *) gcAll_psp.obj

class cBase;
class cFile;

// ============================================================
// 0x0005f788 — eStaticSpotLight::eStaticSpotLight(cBase *)
// ============================================================

class eStaticLight {
public:
    eStaticLight(cBase *);
};

class eStaticSpotLight : public eStaticLight {
public:
    eStaticSpotLight(cBase *);
};

extern char eStaticSpotLightvirtualtable[];

#pragma control sched=1
eStaticSpotLight::eStaticSpotLight(cBase *parent) : eStaticLight(parent) {
    *(void **)((char *)this + 4) = eStaticSpotLightvirtualtable;
    *(float *)((char *)this + 0x90) = 90.0f;
    __asm__ volatile("" ::: "memory");
}
#pragma control sched=2

// ============================================================
// 0x000d4d4c — gcNamedSet::Write(cFile &) const
// ============================================================

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cHandle {
public:
    int mIndex;
    void Write(cWriteBlock &) const;
};

class gcNamedSet {
public:
    cBase *m_parent;     // 0x00
    void *m_vtable;      // 0x04
    cHandle mName;       // 0x08
    void Write(cFile &) const;
};

void gcNamedSet::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    mName.Write(wb);
    wb.End();
}

// ============================================================
// 0x00155eb0 — gcRigidBodyControllerTemplate::gcRigidBodyControllerTemplate(cBase *)
// ============================================================

class gcEntityControllerTemplate {
public:
    gcEntityControllerTemplate(cBase *);
};

class gcRigidBodyControllerTemplate : public gcEntityControllerTemplate {
public:
    gcRigidBodyControllerTemplate(cBase *);
};

extern char gcRigidBodyControllerTemplatevirtualtable[];

gcRigidBodyControllerTemplate::gcRigidBodyControllerTemplate(cBase *parent)
    : gcEntityControllerTemplate(parent) {
    *(void **)((char *)this + 4) = gcRigidBodyControllerTemplatevirtualtable;
    *(int *)((char *)this + 0x18) |= 1;
}
