// Multi-class wrapper file for small leaf-of-leaf methods.
// Functions:
//   gcValHeadset::GetText(char *) const                  @ 0x003472c8
//   eSound::IsPlaying(void) const                        @ 0x00021460
//   gcValLobbyInfo::GetText(char *) const                @ 0x00349db8
//   gcValLobbyOptions::AssignCopy(const cBase *)         @ 0x0034a9c4
//   gcValLobbySessionStatus::GetText(char *) const       @ 0x0034e830

class cBase;

void cStrAppend(char *, const char *, ...);

extern const char gcValHeadset_fmt[];               // @ 0x36F494
extern const char gcValLobbyInfo_fmt[];             // @ 0x36F4EC
extern const char gcValLobbySessionStatus_fmt[];    // @ 0x36F544
extern const char gcValGetText_text[];              // @ 0x36DAF0

// ─────────────────────────────────────────────────────────────────────────
// gcValHeadset::GetText(char *) const @ 0x003472c8
// ─────────────────────────────────────────────────────────────────────────

class gcValHeadset {
public:
    void GetText(char *) const;
};

void gcValHeadset::GetText(char *buf) const {
    cStrAppend(buf, gcValHeadset_fmt, gcValGetText_text);
}

// ─────────────────────────────────────────────────────────────────────────
// eSound::IsPlaying(void) const @ 0x00021460
// ─────────────────────────────────────────────────────────────────────────

class eAudio {
public:
    static int IsPlayingSound(int);
};

class eSound {
public:
    short pad0[24];     // 0x00..0x2F
    short channel;      // 0x30
    bool IsPlaying(void) const;
};

bool eSound::IsPlaying(void) const {
    return (this->channel >= 0) && (eAudio::IsPlayingSound(this->channel) != 0);
}

// ─────────────────────────────────────────────────────────────────────────
// gcValLobbyInfo::GetText(char *) const @ 0x00349db8
// ─────────────────────────────────────────────────────────────────────────

class gcValLobbyInfo {
public:
    void GetText(char *) const;
};

void gcValLobbyInfo::GetText(char *buf) const {
    cStrAppend(buf, gcValLobbyInfo_fmt, gcValGetText_text);
}

// ─────────────────────────────────────────────────────────────────────────
// gcValLobbyOptions::AssignCopy(const cBase *) @ 0x0034a9c4
// ─────────────────────────────────────────────────────────────────────────

class gcValLobbyOptions {
public:
    int pad0;
    int pad4;
    int f8;
    void AssignCopy(const cBase *);
};

gcValLobbyOptions *dcast(const cBase *);

void gcValLobbyOptions::AssignCopy(const cBase *base) {
    gcValLobbyOptions *other = dcast(base);
    this->f8 = other->f8;
}

// ─────────────────────────────────────────────────────────────────────────
// gcValLobbySessionStatus::GetText(char *) const @ 0x0034e830
// ─────────────────────────────────────────────────────────────────────────

class gcValLobbySessionStatus {
public:
    void GetText(char *) const;
};

void gcValLobbySessionStatus::GetText(char *buf) const {
    cStrAppend(buf, gcValLobbySessionStatus_fmt, gcValGetText_text);
}
