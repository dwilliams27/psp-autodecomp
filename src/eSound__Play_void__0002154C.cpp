// eSound::Play(void) @ 0x0002154c, 148B  (eAll_psp.obj)
// Isolated split-TU reconstruction emitting __0fGeSoundEPlayv.
//
// If the sound is not already playing (mChannel < 0), start it via
// eAudio::PlaySound, store the returned channel back into mChannel, and if
// playback succeeded (channel >= 0) push the per-sound frequency and volume
// offsets to the audio system with the corresponding "apply now" flags taken
// from mFlags (bit 3 -> frequency, bit 2 -> volume).
//
// The cTimeValue argument to PlaySound is a zero-initialised stack temporary
// (a2 := sp[0] := 0). PlaySound's static signature is
//   (cHandleT<eSoundData>, eSound*, cTimeValue, eSound*, bool, float, int)
// but its float parameter lowers to $f12 and the trailing int constant 1 to
// $t0; we model the exact register lowering with a flat extern declaration
// since the relocations/symbols are masked by compare_func.

class eSound;

// cTimeValue is a 4-byte by-value struct; a zero-initialised temporary is
// passed for the "start time" argument, which SNC materialises on the stack
// (sw zero,0(sp)) and reloads into a2 (lw a2,0(sp)).
struct cTimeValue {
    int t;
};

// Register lowering of the three eAudio static calls. PlaySound's first FPU
// argument occupies $f12; the integer arguments fill a0..a3 then t0,t1.
extern "C" short eAudio_PlaySound(int, eSound *, cTimeValue, int, int, float, int);
extern "C" void eAudio_SetSoundFrequencyOffset(int, float, bool);
extern "C" void eAudio_SetSoundVolumeOffset(int, float, bool);

class eSound {
public:
    char pad0[0xC];
    unsigned short mFlags;   // 0x0C
    char padE[0x20 - 0x0E];
    int mUnk20;              // 0x20
    char pad24[0x2C - 0x24];
    int mHandle;             // 0x2C
    short mChannel;          // 0x30
    short mUnk32;            // 0x32
    float mFreq;             // 0x34
    float mVol;              // 0x38
    char pad3C[0x40 - 0x3C];
    float mPitch;            // 0x40

    void Play(void);
};

// ── eSound::Play(void) @ 0x0002154c ──
void eSound::Play(void) {
    if (mChannel < 0) {
        cTimeValue start = { 0 };
        mChannel = eAudio_PlaySound(mHandle, this, start, mUnk20, 1, mPitch, mUnk32);
        if (mChannel >= 0) {
            eAudio_SetSoundFrequencyOffset(mChannel, mFreq, (mFlags & 8) != 0);
            eAudio_SetSoundVolumeOffset(mChannel, mVol, (mFlags & 4) != 0);
        }
    }
}
