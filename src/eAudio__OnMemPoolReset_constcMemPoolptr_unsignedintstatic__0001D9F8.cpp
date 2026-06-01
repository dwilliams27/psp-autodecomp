// eAudio::OnMemPoolReset(const cMemPool *, unsigned int) static  @ 0x0001d9f8
// Symbol: __0fGeAudioOOnMemPoolResetPC6IcMemPoolUiT

class cMemPool;

class eAudioChannel {
public:
    void OnMemPoolReset(const cMemPool *pool, unsigned int id);
};

class eAudio {
public:
    static void OnMemPoolReset(const cMemPool *pool, unsigned int id);
};

extern int g_eAudioChannelCount;          // 0x37D0D0
extern char *g_eAudioChannelArray;        // 0x37D0D4

void eAudio::OnMemPoolReset(const cMemPool *pool, unsigned int id) {
    for (int i = 0; i < g_eAudioChannelCount; ++i) {
        eAudioChannel *ch = reinterpret_cast<eAudioChannel *>(
            g_eAudioChannelArray + i * 0x58);
        ch->OnMemPoolReset(pool, id);
    }
}
