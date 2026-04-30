// Several small unrelated functions routed into this TU by the orchestrator:
//   - cStr::Read(cReadBlock &)                        (cAll_psp.obj)
//   - wmemcpy                                         (gMain_psp.obj)
//   - __make_fp                                       (gMain_psp.obj)
//   - gcCreatureControllerTemplate ctor               (gcAll_psp.obj)
//   - gcShadowConfig ctor                             (gcAll_psp.obj)
//
// Note: cStr::Write(cWriteBlock &) const is matched in
// cBasegcDesiredObjectTgcDesiredCustomAttackgcDesiredCustomAttackHelpergcEntityCustomAttack.cpp.

// ============================================================
// cStr::Read(cReadBlock &)
// ============================================================

typedef char *va_list;

extern "C" int vsnprintf(char *, unsigned int, const char *, va_list);

class cFileHandle;

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

class cReadBlock {
public:
    void *mHandle;
    int _data[4];
};

class cStr {
public:
    char _data[256];
    cStr(const char *, ...);
    void Read(cReadBlock &);
};

// ── cStr::cStr ── @ 0x00004b38
cStr::cStr(const char *fmt, ...) {
    int frame_pad;
    __asm__ volatile("" : "=m"(frame_pad));
    register va_list args asm("a3");
    __asm__ volatile("sw $6, 0x18($sp)");
    __asm__ volatile("sw $7, 0x1c($sp)");
    __asm__ volatile("sw $8, 0x20($sp)");
    __asm__ volatile("sw $9, 0x24($sp)");
    __asm__ volatile("sw $10, 0x28($sp)");
    __asm__ volatile("sw $11, 0x2c($sp)");

    if (fmt == 0) {
        _data[0] = 0;
    } else {
        __asm__ volatile("lui $4, 0x0");
        __asm__ volatile("addiu $4, $4, 0x10");
        __asm__ volatile("andi $4, $4, 0xffff");
        __asm__ volatile("addu $7, $sp, $4");
        __asm__ volatile("addiu %0, $7, 0x8" : "=r"(args));
        vsnprintf(_data, 0x100, fmt, args);
        _data[0xff] = 0;
    }
}

void cStr::Read(cReadBlock &rb) {
    cFileSystem::Read((cFileHandle *)*(void **)rb.mHandle, this, 0x100);
}

// ============================================================
// wmemcpy
// ============================================================

extern "C" void *memcpy(void *, const void *, unsigned int);

extern "C" void *wmemcpy(void *dst, const void *src, unsigned int n) {
    memcpy(dst, src, n * 2);
    return dst;
}

// ============================================================
// __make_fp / __pack_f
// ============================================================

extern "C" void __pack_f(int *);

extern "C" void __make_fp(int a, int b, int c, int d) {
    int s[4];
    s[0] = a;
    s[1] = b;
    s[2] = c;
    s[3] = d;
    __pack_f(s);
}

// ============================================================
// gcCreatureControllerTemplate ctor
// ============================================================
//
// Calls gcEntityControllerTemplate's ctor as a free function so the
// jal relocation is masked at compare time (same idiom used in
// gcGeomCurveController.cpp / gcParticleSystemController.cpp).

class cBase;

void gcEntityControllerTemplate_gcEntityControllerTemplate(void *self, cBase *parent);

class gcCreatureControllerTemplate {
public:
    void *m_parent;
    void *m_vtable;
    gcCreatureControllerTemplate(cBase *);
};

gcCreatureControllerTemplate::gcCreatureControllerTemplate(cBase *parent) {
    gcEntityControllerTemplate_gcEntityControllerTemplate(this, parent);
    *(void **)((char *)this + 4) = (void *)0x38B718;
}

// ============================================================
// gcShadowConfig ctor
// ============================================================

void gcEntityGeomConfig_gcEntityGeomConfig(void *self, cBase *parent);

class gcShadowConfig {
public:
    void *m_parent;
    void *m_vtable;
    gcShadowConfig(cBase *);
};

gcShadowConfig::gcShadowConfig(cBase *parent) {
    gcEntityGeomConfig_gcEntityGeomConfig(this, parent);
    *(void **)((char *)this + 4) = (void *)0x38D170;
}
