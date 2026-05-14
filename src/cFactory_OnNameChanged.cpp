// ODR-WARNING: this TU redeclares cFactory with a minimal member list so SNC's
// register allocator for OnNameChanged does not perturb sibling methods in
// cFactory.cpp. Adding `void OnNameChanged(void);` to cFactory.cpp's local
// redeclaration regressed FindObject and DeleteGroups. Do NOT add new methods
// to cFactory's canonical header or to cFactory.cpp's local class.

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cObject {
public:
    cObject(class cBase *);
};

class cFactory : public cObject {
public:
    void OnNameChanged(void);
};

extern "C" int cStrLength(const char *);
extern "C" void cStrCopy(char *, const char *, int);

void cFactory::OnNameChanged(void) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(this);
    char *name = (char *)pool + 4;
    cStrCopy(name, (const char *)this + 8, 0x20);
    ((char *)pool)[35] = 0;
    int len = cStrLength(name);

    int var_a3;
    int var_a0 = len - 1;
    unsigned int crc = (unsigned int)-1;
    const unsigned char *p = (const unsigned char *)name;
    var_a3 = var_a0;
    if (len != 0) {
        int old_a0;
        do {
            unsigned int t1 = *(unsigned int *)(((crc ^ *p) & 0xff) * 4 + 0x37BC50);
            old_a0 = var_a3;
            var_a3 = var_a0 - 1;
            p++;
            crc = t1 ^ (crc >> 8);
            __asm__ volatile("" ::: "memory");
            var_a0 = var_a3;
        } while (old_a0 != 0);
    }
    *(unsigned int *)pool = crc ^ (unsigned int)-1;
}
