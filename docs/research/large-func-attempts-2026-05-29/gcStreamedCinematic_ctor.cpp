// gcStreamedCinematic::gcStreamedCinematic(cBase *, bool) @ 0x00161c00, 4108B, gcAll_psp.obj
// Split-TU. Constructor: base ctor, vtable, gcCinematic subobject ctor, pool GetValue x2,
// then 27 allocate-and-init blocks (one per resource group type).

class cBase;

struct cGUID {
    unsigned int b;
    unsigned int a;
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
    void *GetValue(const cGUID &, void **);
};

extern "C" {
    void cObject_ctor(void *thisptr, cBase *parent) asm("_ZN7cObjectC1EP5cBase");
    void gcCinematic_ctor(void *thisptr, cBase *parent) asm("_ZN11gcCinematicC1EP5cBase");
}

typedef void *(*allocFnT)(void *, int, int, int, int);
struct AllocRec {
    short offset;
    short pad;
    allocFnT fn;
};

struct PoolInfo {
    char pad00[0x1C];
    char *table;   // 0x1C
};

extern "C" {
    int gcStringTableGroup_IsManagedTypeExternalStatic();
    int eSoundDataGroup_IsManagedTypeExternalStatic();
    int eAnimationGroup_IsManagedTypeExternalStatic();
    int gcEntityCustomAnimationGroup_IsManagedTypeExternalStatic();
    int eTextureGroup_IsManagedTypeExternalStatic();
    int eMaterialGroup_IsManagedTypeExternalStatic();
    int eMaterialSetGroup_IsManagedTypeExternalStatic();
    int eMeshGroup_IsManagedTypeExternalStatic();
    int eSpriteGroup_IsManagedTypeExternalStatic();
    int eFontGroup_IsManagedTypeExternalStatic();
    int eSurfaceGroup_IsManagedTypeExternalStatic();
    int eSurfaceSetGroup_IsManagedTypeExternalStatic();
    int eSkinGroup_IsManagedTypeExternalStatic();
    int gcConstantGroup_IsManagedTypeExternalStatic();
    int gcEnumerationGroup_IsManagedTypeExternalStatic();
    int gcFunctionGroup_IsManagedTypeExternalStatic();
    int gcStateMachineGroup_IsManagedTypeExternalStatic();
    int gcUIDialogGroup_IsManagedTypeExternalStatic();
    int gcEntityTemplateGroup_IsManagedTypeExternalStatic();
    int eGeomTemplateGroup_IsManagedTypeExternalStatic();
    int eCameraEffectGroup_IsManagedTypeExternalStatic();
    int ePathGroup_IsManagedTypeExternalStatic();
    int eLensFlareGroup_IsManagedTypeExternalStatic();
    int ePointGroup_IsManagedTypeExternalStatic();
    int gcEntityGroup_IsManagedTypeExternalStatic();
    int gcExternalCinematicGroup_IsManagedTypeExternalStatic();
    int gcEntityCustomAttackGroup_IsManagedTypeExternalStatic();
}

class gcStreamedCinematic {
public:
    gcStreamedCinematic(cBase *, bool);
};

struct Proxy {
    cBase *owner;            // 0x00
    unsigned int vtbl;       // 0x04
    unsigned char managed;   // 0x08
    char pad[3];
    unsigned int unkC;       // 0x0C
};

typedef int (*isManagedFnT)();

// Inlined helper, mirrors the per-group ctor chain. Result var begins NULL,
// trailing store is unconditional (drives beqzl). The three vtbl writes model
// base/mid/leaf ctor vptr stores; written through a volatile lvalue so SNC keeps
// all three across the (inlined) ctor boundaries.
static inline Proxy *alloc_proxy(cMemPool *pool, void *owner, isManagedFnT isManaged, unsigned int vtbl) {
    PoolInfo *pi = ((PoolInfo **)pool)[9];          // pool->unk24
    char *table = pi->table;                        // pi->unk1C
    Proxy *result = 0;
    AllocRec *rec = (AllocRec *)((unsigned)table + 0x28);
    Proxy *obj = (Proxy *)rec->fn((char *)pi + rec->offset, 0x10, 4, 0, 0);
    if (obj != 0) {
        int m = 0;
        int isMan = isManaged();
        unsigned int v0c = 0x37E6A8;                 // base ctor vtbl
        unsigned int v1c = 0x37EA80;                 // mid ctor vtbl
        if (isMan == 0) {
            m = 1;
        }
        volatile Proxy *vo = obj;
        vo->vtbl = v1c;
        obj->unkC = 0;
        vo->vtbl = v0c;
        obj->managed = (unsigned char)((m & 0xFF) & 0xFF);
        obj->owner = (cBase *)owner;
        vo->vtbl = vtbl;                             // leaf vtbl
        result = obj;
    }
    return result;
}

static int g_idx asm("D_0037D7D0");
static cBase *g_table[1] asm("D_0037D7D4");

gcStreamedCinematic::gcStreamedCinematic(cBase *parent, bool flag) {
    cObject_ctor(this, parent);                     // base ctor

    *(int *)((char *)this + 0x44) = 1;
    *(int *)((char *)this + 0x48) = 0x800;
    *(unsigned int *)((char *)this + 0x04) = 0x387688;   // vtable

    gcCinematic_ctor((char *)this + 0x4C, (cBase *)this);  // member subobject ctor (parent = this)

    *(int *)((char *)this + 0xCC) = 0;
    *(char *)((char *)this + 0xD0) = (char)flag;
    *(char *)((char *)this + 0x140) = 0;
    *(char *)((char *)this + 0x141) = 0;

    int idx = (g_idx == 0) ? 1 : 0;
    *(int *)((char *)this + 0x144) = idx;
    g_table[idx] = (cBase *)this;

    cMemPool *pool = cMemPool::GetPoolFromPtr(this);
    ((cBase **)pool)[10] = (cBase *)this;           // pool->unk28 = this

    cGUID g0a; g0a.a = 0x812D2B72; g0a.b = 0xB1E8A1F9;
    void *out0 = 0;
    void *out1 = 0;
    cGUID g0b; g0b.a = 0x812D2B72; g0b.b = 0xB1E8A1F9;
    cGUID g1a; g1a.a = 0x812D2B72; g1a.b = 0xB1E8A1F9;
    cGUID g0c; g0c.a = g0b.a; g0c.b = g0b.b;
    pool->GetValue(g0c, &out0);
    cGUID g1b; g1b.a = 0x812D2B72; g1b.b = 0xB1E8A1F9;
    cGUID g1c; g1c.a = g1b.a; g1c.b = g1b.b;
    pool->GetValue(g1c, &out1);
    ((cBase **)out0)[10] = (cBase *)this;           // out0->unk28
    ((cBase **)out1)[10] = (cBase *)this;           // out1->unk28

    *(void **)((char *)this + 0xD4) = alloc_proxy(pool, this, gcStringTableGroup_IsManagedTypeExternalStatic, 0x3862D0);
    *(void **)((char *)this + 0xD8) = alloc_proxy(pool, this, eSoundDataGroup_IsManagedTypeExternalStatic, 0x37EE90);
    *(void **)((char *)this + 0xDC) = alloc_proxy(pool, this, eAnimationGroup_IsManagedTypeExternalStatic, 0x37EF20);
    *(void **)((char *)this + 0xE0) = alloc_proxy(pool, this, gcEntityCustomAnimationGroup_IsManagedTypeExternalStatic, 0x386870);
    *(void **)((char *)this + 0xE4) = alloc_proxy(pool, this, eTextureGroup_IsManagedTypeExternalStatic, 0x37EB30);
    *(void **)((char *)this + 0xE8) = alloc_proxy(pool, this, eMaterialGroup_IsManagedTypeExternalStatic, 0x37EBC0);
    *(void **)((char *)this + 0xEC) = alloc_proxy(pool, this, eMaterialSetGroup_IsManagedTypeExternalStatic, 0x37EE00);
    *(void **)((char *)this + 0xF0) = alloc_proxy(pool, this, eMeshGroup_IsManagedTypeExternalStatic, 0x37ECE0);
    *(void **)((char *)this + 0xF4) = alloc_proxy(pool, this, eSpriteGroup_IsManagedTypeExternalStatic, 0x37F0D0);
    *(void **)((char *)this + 0xF8) = alloc_proxy(pool, this, eFontGroup_IsManagedTypeExternalStatic, 0x37F160);
    *(void **)((char *)this + 0xFC) = alloc_proxy(pool, this, eSurfaceGroup_IsManagedTypeExternalStatic, 0x37EFB0);
    *(void **)((char *)this + 0x100) = alloc_proxy(pool, this, eSurfaceSetGroup_IsManagedTypeExternalStatic, 0x37F040);
    *(void **)((char *)this + 0x104) = alloc_proxy(pool, this, eSkinGroup_IsManagedTypeExternalStatic, 0x37F280);
    *(void **)((char *)this + 0x108) = alloc_proxy(pool, this, gcConstantGroup_IsManagedTypeExternalStatic, 0x386240);
    *(void **)((char *)this + 0x10C) = alloc_proxy(pool, this, gcEnumerationGroup_IsManagedTypeExternalStatic, 0x3865A0);
    *(void **)((char *)this + 0x110) = alloc_proxy(pool, this, gcFunctionGroup_IsManagedTypeExternalStatic, 0x386120);
    *(void **)((char *)this + 0x114) = alloc_proxy(pool, this, gcUIDialogGroup_IsManagedTypeExternalStatic, 0x386360);
    *(void **)((char *)this + 0x118) = alloc_proxy(pool, this, gcStateMachineGroup_IsManagedTypeExternalStatic, 0x386630);
    *(void **)((char *)this + 0x11C) = alloc_proxy(pool, this, eGeomTemplateGroup_IsManagedTypeExternalStatic, 0x37ED70);
    *(void **)((char *)this + 0x120) = alloc_proxy(pool, this, gcEntityTemplateGroup_IsManagedTypeExternalStatic, 0x386480);
    *(void **)((char *)this + 0x124) = alloc_proxy(pool, this, eCameraEffectGroup_IsManagedTypeExternalStatic, 0x37F4C0);
    *(void **)((char *)this + 0x128) = alloc_proxy(pool, this, eLensFlareGroup_IsManagedTypeExternalStatic, 0x37F430);
    *(void **)((char *)this + 0x12C) = alloc_proxy(pool, this, ePathGroup_IsManagedTypeExternalStatic, 0x37EC50);
    *(void **)((char *)this + 0x130) = alloc_proxy(pool, this, ePointGroup_IsManagedTypeExternalStatic, 0x37F1F0);
    *(void **)((char *)this + 0x134) = alloc_proxy(pool, this, gcEntityGroup_IsManagedTypeExternalStatic, 0x3863F0);
    *(void **)((char *)this + 0x138) = alloc_proxy(pool, this, gcExternalCinematicGroup_IsManagedTypeExternalStatic, 0x386AB0);
    *(void **)((char *)this + 0x13C) = alloc_proxy(pool, this, gcEntityCustomAttackGroup_IsManagedTypeExternalStatic, 0x386900);
}
