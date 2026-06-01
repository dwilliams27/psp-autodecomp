// ODR-WARNING: split-TU local redeclaration of nwConfigBase for isolated build.
// nwConfigBase::AssignCopy(const cBase *) @ 0x0036b340
class cBase;

class nwConfigBase {
public:
    void AssignCopy(const cBase *);
};

extern "C" nwConfigBase *dcastdcast_nwConfigBaseptr__constcBaseptr__0036B228(const cBase *);

void nwConfigBase::AssignCopy(const cBase *src) {
    nwConfigBase *other =
        dcastdcast_nwConfigBaseptr__constcBaseptr__0036B228(src);

    *(int *)((char *)this + 0x8) = *(int *)((char *)other + 0x8);
    *(unsigned short *)((char *)this + 0xC) =
        *(unsigned short *)((char *)other + 0xC);
    *(int *)((char *)this + 0x10) = *(int *)((char *)other + 0x10);
    *(unsigned char *)((char *)this + 0x14) =
        *(unsigned char *)((char *)other + 0x14);
    *(int *)((char *)this + 0x18) = *(int *)((char *)other + 0x18);
    *(int *)((char *)this + 0x1C) = *(int *)((char *)other + 0x1C);

    int i = 0;
    do {
        *((char *)this + (0x20 + i)) = *(i + 0x20 + (char *)other);
        i += 1;
    } while (i < 0x20);

    int j = 0;
    do {
        *((char *)this + (0x40 + j)) = *(j + 0x40 + (char *)other);
        j += 1;
    } while (j < 0x20);
}
