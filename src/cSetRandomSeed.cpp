extern unsigned int gRandomSeed1;
extern unsigned int gRandomSeed2;

extern "C" void cSetRandomSeed(unsigned int seed1, unsigned int seed2) {
    gRandomSeed1 = seed1;
    gRandomSeed2 = seed2;
}
