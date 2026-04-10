extern "C" {

extern char *g_eAudio_channels;

int eAudio__GetSoundTime_intstatic(int arg0) {
    volatile int result;
    char *p = g_eAudio_channels + arg0 * 0x58;
    int flags = *(int *)p & 3;
    flags = (flags != 0) & 0xFF;
    p += 4;
    if (!flags) {
        result = 0;
        return result;
    }
    return *(int *)p;
}

}
