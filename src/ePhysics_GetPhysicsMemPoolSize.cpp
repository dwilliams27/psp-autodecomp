struct ePhysCfg {
    char _pad0[0x60];
    int n60;
    int n64;
    int n68;
};

extern "C" {

int ePhysics__GetPhysicsMemPoolSize_voidstatic(void) {
    ePhysCfg *p = *(ePhysCfg **)0x37D0C8;
    return p->n68 * 20 + p->n60 * 80 + p->n64 * 112 + 0x145C;
}

}
