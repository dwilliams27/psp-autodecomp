// eDynamicLight::Reset(cMemPool*, bool) @ 0x00077924
// Split-TU: declare class locally to emit __0fNeDynamicLightFResetP6IcMemPoolb.

class cMemPool;

struct eDynamicLightTarget {
    char pad0[76];
    float val;   // offset 76
};

class eDynamicLight {
public:
    void Reset(cMemPool* pool, bool flag);
private:
    char pad0[96];
    eDynamicLightTarget* target;  // offset 96
    char pad60[16];               // 100..115
    float result;                 // offset 116
    char pad74[20];               // 120..139
    char someByte;                // offset 140
    char flagByte;                // offset 141
};

#pragma control sched=1
void eDynamicLight::Reset(cMemPool* pool, bool flag) {
    flagByte = 1;
    __asm__ volatile("" ::: "memory");
    if (target != 0) {
        result = target->val;
        return;
    }
    result = -1.0f;
}
