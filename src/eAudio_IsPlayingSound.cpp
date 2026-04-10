extern "C" {

extern char *g_eAudio_channels;

int eAudio__IsPlayingSound_intstatic(int arg0) {
    return (*(int *)(g_eAudio_channels + arg0 * 0x58) & 3) != 0;
}

}
