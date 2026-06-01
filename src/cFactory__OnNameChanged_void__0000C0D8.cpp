// cFactory::OnNameChanged(void)
// Address: 0x0000c0d8, Obj: cAll_psp.obj
// Symbol: __0fIcFactoryNOnNameChangedv
// ODR-WARNING: split-TU local class redeclaration; do not add to include/*.h.

class cFactory {
public:
    void OnNameChanged(void);
};

extern unsigned int gCrcTable[256]; // at 0x0037bc50

struct cMemPool {
    static void *GetPoolFromPtr(const void *);
};

char *cStrCopy(char *, const char *, int);
int cStrLength(const char *);

void cFactory::OnNameChanged(void) {
    char *pool = (char *)cMemPool::GetPoolFromPtr(this);
    char *name = pool + 4;
    cStrCopy(name, (const char *)this + 8, 0x20);
    pool[35] = 0;

    int count = cStrLength(name);
    unsigned int crc = 0xFFFFFFFFu;
    const unsigned char *p = (const unsigned char *)name;
    while (count-- != 0) {
        crc = gCrcTable[(crc ^ *p++) & 0xFF] ^ (crc >> 8);
    }
    *(int *)pool = crc ^ 0xFFFFFFFFu;
}
