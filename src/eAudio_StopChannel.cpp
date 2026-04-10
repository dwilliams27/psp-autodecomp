extern "C" {

extern int sceSasSetKeyOff(int iVoiceNum);
void eAudioPlatform__StopStream_intstatic(int);

extern char g_eAudio_channelActive[];

void eAudio__StopChannel_consteAudioChannelptrstatic(void *arg0) {
    int ch = *(int *)((char *)arg0 + 0x54) - 1;
    if (ch >= 0) {
        if (ch < 32) {
            sceSasSetKeyOff(ch);
        } else {
            eAudioPlatform__StopStream_intstatic(ch);
        }
        g_eAudio_channelActive[ch] = 0;
    }
}

}
