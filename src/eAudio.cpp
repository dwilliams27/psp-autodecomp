extern int g_eAudio_updateCount;

extern "C" {

void eAudio__PlatformUpdate_voidstatic(void) {
    g_eAudio_updateCount += 1;
}

}
