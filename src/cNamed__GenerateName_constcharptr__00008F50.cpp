// cNamed::GenerateName(const char *)  @ 0x00008f50, 224B  (cAll_psp.obj)
// Isolated split-TU build of GenerateName only.

class cName {
public:
    void Set(const char *, ...);
};

unsigned int cIRand(void);
int cStrLength(const char *);
void cStrCopy(char *, const char *, int);
void cStrAppend(char *, const char *, ...);

extern const char cNamed_genname_fmt[];   // @ 0x36CA64

struct DispatchEntry {
    short offset;
    short pad;
    void (*fn)(void *);
};

class cBase {
public:
    cBase *mOwner;          // 0
    void *mClassDesc;       // 4
};

struct cNameData { int _w[6]; };  // 24-byte name buffer at cNamed+8

class cNamed : public cBase {
public:
    cNameData mName;        // 8..0x20
    void GenerateName(const char *);
};

void cNamed::GenerateName(const char *src) {
    char buf[256];
    int n = cStrLength(src) - 1;
test:
    if (n >= 0) {
        int c = ((int)src[n] << 24) >> 24;
        if (((c >= '0') & (c < ':')) & 0xff) {
            n = n - 1;
            goto test;
        }
    }
    buf[0] = 0;
    int len = 18;
    n = n + 2;
    if (n < 18) len = n;
    cStrCopy(buf, src, len);
    cStrAppend(buf, cNamed_genname_fmt, cIRand() % 100);
    ((cName *)((char *)this + 8))->Set(buf);
    DispatchEntry *e = (DispatchEntry *)(*(char **)((char *)this + 4) + 0x70);
    e->fn((char *)this + e->offset);
}
