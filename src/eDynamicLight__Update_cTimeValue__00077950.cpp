// eDynamicLight::Update(cTimeValue) @ 0x00077950
// Split-TU: declare class locally to emit __0fNeDynamicLightGUpdate6KcTimeValue.

class cTimeValue { int v; };

struct eDynamicLightDelegate {
    short offset;          // 0xB8 (s16)
    char pad2[2];
    void (*fn)(void*);     // 0xB8+4
};

struct eDynamicLightObj {
    char pad0[0xB8];
    eDynamicLightDelegate dg;  // 0xB8
};

class eDynamicLight {
public:
    void Update(cTimeValue t);
private:
    char pad0[4];
    eDynamicLightObj* obj;   // offset 4
    char pad8[0x84];         // 8..0x8B
    unsigned char flags;     // 0x8C
};

#pragma control sched=1
void eDynamicLight::Update(cTimeValue t) {
    if (flags & 4) {
        eDynamicLightDelegate* d = &obj->dg;
        d->fn((char*)this + d->offset);
    }
}
