class eMoviePlatform {
public:
    void soundbuf_setPts(unsigned int pts);
};

void eMoviePlatform::soundbuf_setPts(unsigned int pts) {
    if (pts == (unsigned int)-1) {
        pts = *(unsigned int *)((char *)this + 0x33C) + *(unsigned int *)((char *)this + 0x34C);
    }
    *(unsigned int *)((char *)this + 0x33C) = pts;
    int idx = *(int *)((char *)this + 0x328);
    *(unsigned int *)((char *)this + idx * 4 + 0x318) = pts;
}
