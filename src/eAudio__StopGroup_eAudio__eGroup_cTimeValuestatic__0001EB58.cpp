// eAudio::StopGroup(eAudio::eGroup, cTimeValue) static
// Address: 0x0001eb58, Size: 112B, Obj: eAll_psp.obj

class cTimeValue {
public:
    int mTime;
};

class eAudio {
public:
    enum eGroup { kGroupDummy };
    static void StopGroup(eGroup group, cTimeValue tv);
    static void StopSound(int sound, cTimeValue tv);
};

struct eAudioGroupEntry {
    int mFirst;     // 0x00
    int mLast;      // 0x04
};

extern char g_audioGroupTable[];

void eAudio::StopGroup(eGroup group, cTimeValue tv)
{
    eAudioGroupEntry *e = (eAudioGroupEntry *)((int)group * 0x38 + (int)g_audioGroupTable);
    for (int i = e->mFirst; e->mLast >= i; i++) {
        StopSound(i, tv);
    }
}
