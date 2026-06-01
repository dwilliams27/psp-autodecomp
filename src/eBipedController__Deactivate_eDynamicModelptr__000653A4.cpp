// eBipedController::Deactivate(eDynamicModel*) @ 0x000653a4
// Split-TU: classes declared locally to avoid editing shared headers.
// eAll_psp.obj engine zone (sched=1).
//
// Layout (from disasm 000653a4):
//   this->index   @ 0x10  (signed; <0 => return)
//   this->entries @ 0x30  (array base; count packed at entries[-1] & 0x3FFFFFFF)
//   each entry is 8 bytes: { void* shape @0, int @4 }
//   this->field44 @ 0x44, this->field48 @ 0x48 (cleared to 0)

class eDynamicModel;

class ePhysicsController {
public:
    unsigned char _basePad[0x10];
};

struct eBipedEntry {
    void *shape;   // offset 0  (-> SetShapeController arg)
    int   field4;  // offset 4
};

class eBipedController {
public:
    char _pad0[0x10];
    int          index;     // offset 0x10
    char _pad14[0x30 - 0x14];
    eBipedEntry *entries;    // offset 0x30
    char _pad34[0x44 - 0x34];
    int          field44;    // offset 0x44
    int          field48;    // offset 0x48
public:
    void Deactivate(eDynamicModel *);
};

class ePhysics {
public:
    static ePhysics *Get();
    void RemoveFromUpdateList(eBipedController *);
};

extern void eDynamicModel__SetShapeController(eDynamicModel *, void *,
                                              ePhysicsController *, unsigned int);

#pragma control sched=1
void eBipedController::Deactivate(eDynamicModel *model) {
    int idx = this->index;
    if (idx < 0) {
        return;
    }

    eBipedEntry *entries = this->entries;
    int count = 0;
    if (entries != 0) {
        count = ((int *)entries)[-1] & 0x3FFFFFFF;
    }

    if (idx < count) {
        eBipedEntry *e = entries + idx;
        int active = 0;
        if (e->shape != 0) {
            if (e->field4 != 0) {
                active = 1;
            }
        }
        if ((active & 0xff) != 0) {
            this->field44 = 0;
            this->field48 = 0;
            eDynamicModel__SetShapeController(model, e->shape, 0, 0);
            ePhysics::Get()->RemoveFromUpdateList(this);
        }
    }
}
