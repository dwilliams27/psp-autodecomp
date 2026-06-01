// gcEntity::IsLocallyControlled(void) const  @ 0x001193e4
// Split-TU isolated file. Symbol: __0fIgcEntityTIsLocallyControlledvK
//
// NOTE: sibling file src/gcValEntityIsLocallyControlled.cpp also declares
// gcEntity::IsLocallyControlled but does NOT define it, so no duplicate symbol.

class gcStreamedCinematic {
public:
    static int HasFullscreenStarted(void);
};

class gcEntity {
public:
    char _pad0[0x5C];
    int mFlags;   // 0x5C
    char _pad1[0x68 - 0x60];
    char mOwner;  // 0x68
    bool IsA(void) const { return (mFlags & 0x10000) != 0; }
    bool IsB(void) const { return (mFlags & 0x4000) != 0; }
    int IsLocallyControlled(void) const;
};

extern int g_word_37D858;
extern int g_word_37D85C;

int gcEntity::IsLocallyControlled(void) const
{
    if ((g_word_37D858 & 1) == 0 || IsA() || IsB()) {
        return 1;
    }
    int fs = gcStreamedCinematic::HasFullscreenStarted();
    char owner = mOwner;
    if (fs != 0) {
        goto fullscreen;
    }
common:
    return owner == g_word_37D85C;
fullscreen:
    if (owner != 0) {
        goto common;
    }
    return 1;
}
