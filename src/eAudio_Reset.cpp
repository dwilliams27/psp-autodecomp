extern "C" {

void eAudioGroup__ResetUserFaders_void(void *);

void eAudio__Reset_voidstatic(void) {
    unsigned int addr = 0x40E88;
    int i = 0;
    char *p = (char *)0;
    p = p + addr;
    do {
        eAudioGroup__ResetUserFaders_void(p);
        i += 1;
        p += 0x38;
    } while (i < 4);
}

}
