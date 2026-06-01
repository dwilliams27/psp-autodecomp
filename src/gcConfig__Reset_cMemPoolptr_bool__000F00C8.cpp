// gcConfig::Reset(cMemPool *, bool) @ 0x000f00c8 — split-TU isolation file.
// Symbol: __0fIgcConfigFResetP6IcMemPoolb
//
// Resets the configuration block: recompute the preset memory sizes, reset
// the global timescale to 1.0, and recompute the rounded tick budget from the
// engine frame-rate constant at 0x40E7C.

class cBase;
class cMemPool;

class gcConfig {
public:
    void Reset(cMemPool *pool, bool flag);
    void CalcPresetSizes(void);
};

void gcConfig::Reset(cMemPool *pool, bool flag) {
    float half = 0.5f;

    CalcPresetSizes();

    float one = 1.0f;
    int raw = *(int *)0x40E7C;
    *(float *)0x37D7E0 = one;

    float t = one * (float)raw;

    int stk[3];
    stk[1] = raw;

    int rounded;
    if (t < 0.0f) {
        rounded = (int)(t - half);
    } else {
        rounded = (int)(t + 0.5f);
    }

    stk[0] = rounded;
    __asm__ volatile("" :: "r"(&stk[0]));
    *(int *)0x99AF4 = rounded;
}
