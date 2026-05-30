// eAudio::GetGroupVolume(eAudio::eGroup, eAudioGroup::eFader) static
// Address: 0x0001efb0, Size: 44B, Obj: eAll_psp.obj

class eAudioGroup {
public:
    enum eFader { kFaderDummy };
};

struct LeAudioFader {
    int   unk0;
    float volume;
};

class eAudio {
public:
    enum eGroup { kGroupDummy };
    static float GetGroupVolume(eGroup group, eAudioGroup::eFader fader);
};

extern LeAudioFader g_audioGroupTable[];

float eAudio::GetGroupVolume(eGroup group, eAudioGroup::eFader fader)
{
    int off = (int)group * 0x38 + (int)((char *)g_audioGroupTable);
    LeAudioFader *f = (LeAudioFader *)(off + (int)fader * 0x10 + 8);
    return f->volume;
}
