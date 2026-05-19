// ODR-WARNING: split translation unit for one gcValEntitySoundFrequency method.
// Keep this local redeclaration minimal so matched siblings in the canonical TU
// do not pick up register-allocation drift.

class gcEntity {
public:
    float GetSoundFrequencyOffset(int) const;
    float GetSoundVolumeOffset(int) const;
};

class gcDesiredEntity {
public:
    gcEntity *Get(bool) const;
};

class gcValEntitySoundFrequency {
public:
    float Evaluate(void) const;
};

struct EvalSlot {
    short offset;
    short pad;
    void *fn;
};

extern "C" double log(double);

static float soundLogScale asm("D_00099898");
static int soundLogScaleInitialized asm("D_0009989C");
extern const double soundLogMax asm("D_0036F3A8");

float gcValEntitySoundFrequency::Evaluate(void) const {
    gcEntity *entity =
        ((const gcDesiredEntity *)((const char *)this + 8))->Get(true);
    if (entity == 0) {
        return 0.0f;
    }

    int value = *(int *)((const char *)this + 0x34);
    int tagged = value & 1;

    if (*(unsigned char *)((const char *)this + 0x38) != 0) {
        int flag = 0;
        if (tagged != 0) {
            flag = 1;
        }
        if (flag != 0) {
            value = 0;
        } else {
            __asm__ volatile("" ::: "memory");
        }

        int index;
        int check = value;
        if (check != 0) {
            EvalSlot *slot = (EvalSlot *)(*(char **)(check + 4) + 0x70);
            short off = slot->offset;
            float (*fn)(void *) = (float (*)(void *))slot->fn;
            index = (int)fn((char *)value + off);
        } else {
            float zero = 0.0f;
            index = (int)zero;
        }

        return entity->GetSoundVolumeOffset(index);
    }

    int flag = 0;
    if (tagged != 0) {
        flag = 1;
    }
    if (flag != 0) {
        value = 0;
    } else {
        __asm__ volatile("" ::: "memory");
    }

    int index;
    int check = value;
    if (check != 0) {
        EvalSlot *slot = (EvalSlot *)(*(char **)(check + 4) + 0x70);
        short off = slot->offset;
        float (*fn)(void *) = (float (*)(void *))slot->fn;
        index = (int)fn((char *)value + off);
    } else {
        float zero = 0.0f;
        index = (int)zero;
    }

    float frequency = entity->GetSoundFrequencyOffset(index);
    if (*(unsigned char *)((const char *)this + 0x39) != 0) {
        if (soundLogScaleInitialized == 0) {
            soundLogScaleInitialized = 1;
            soundLogScale = 1.0f / (float)log(soundLogMax);
        }
        return (float)log((double)frequency) * soundLogScale;
    }
    return frequency;
}
