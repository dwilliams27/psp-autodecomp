// ODR-WARNING: this TU intentionally redeclares eDynamicVertChunk with a
// minimal member list (matching the field layout used by Draw) so SNC's
// register allocator and constant scheduling for the Draw body don't see the
// rest of the class context that already produces matching constructors and
// Begin* methods in eDynamicVertChunk.cpp. DO NOT add `#include` for any
// eDynamicVertChunk header; add new methods to this local class only if
// their bodies are emitted in this TU.

class eDynamicVertChunk {
public:
    void *mBuffer;       // +0
    int *mPtr;           // +4 (top byte = type tag, low 24 bits = buffer index)
    int mCount;          // +8
    int mType;           // +C

    void Draw(void);
};

struct eDynamicVertChunkPool {
    char pad0[8];
    int *list;           // +8
};
extern eDynamicVertChunkPool D_000984D0;
extern unsigned int D_0037D108;
extern unsigned int D_0037D11C;
extern unsigned int D_0037D118;
extern unsigned int D_0037D36C;
extern unsigned int D_0037D370;

void eDynamicVertChunk::Draw(void) {
    int t1 = 4;
    int t0 = 0;
    int type = mType;
    unsigned int a3 = 0;
    int a2 = 0;
    int a1 = 0;
    int v0 = 0x19F;
    int *p = D_000984D0.list;

    switch (type) {
    case 0:
        a1 = mCount;
        t1 = 4;
        a3 = a1 / 4;
        t0 = 4;
        a2 = a3 * 2;
        a1 = a3 * 4;
        break;
    case 1:
        a1 = mCount;
        t0 = a1;
        t1 = 4;
        a3 = 1;
        a2 = a1 - 2;
        break;
    case 2:
        a1 = mCount;
        t1 = 3;
        t0 = 3;
        a3 = a1 / 3;
        a2 = a3;
        a1 = (a3 + a3 + a3);
        break;
    case 3:
        a1 = mCount;
        t0 = a1;
        t1 = 5;
        a3 = 1;
        a2 = a1 - 2;
        break;
    case 4:
        a1 = mCount;
        t1 = 1;
        t0 = a1;
        a3 = 1;
        D_0037D118 += a1 / 2;
        a2 = 0;
        break;
    case 5:
        a1 = mCount;
        t1 = 3;
        t0 = 3;
        v0 = 0x180;
        a3 = a1 / 3;
        a2 = a3;
        a1 = a3 * 3;
        break;
    }

    if ((D_0037D36C | D_0037D370) != 0) {
        D_000984D0.list = p + 1;
        *p = (int)0xC8000000;
        D_0037D36C = 0;
        D_0037D370 = 0;
        ++p;
    }

    unsigned int packed = (unsigned int)mPtr;
    D_000984D0.list = (a3 + 3) + p;
    p[0] = v0 | 0x12000000;
    p[1] = (int)((((packed >> 24) & 0xF) << 16) | 0x10000000);
    p[2] = (int)((packed & 0xFFFFFF) | 0x01000000);

    int *q = 3 + p;
    int i = 0;
    if (i < (int)a3) {
        int word = (t1 << 16) | 0x04000000 | t0;
        do {
            *q = word;
            q++;
            i++;
        } while (i < (int)a3);
    }

    D_0037D108 += a2;
    D_0037D11C += a1;
}
