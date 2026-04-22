class cReadBlock {
public:
    int _data[5];
};

class cInStream;
class cOutStream;
class cWriteBlock;

class cFileSystem {
public:
    static void Read(void *handle, void *buf, unsigned int size);
};

class cOutStreamRef {
public:
    void Write(unsigned int, int, bool);
};

class cInStreamRef {
public:
    void Read(unsigned int &, int, bool);
};

extern "C" int cIRand(void);

struct cGUID {
    int a;
    int b;
    static cGUID Generate();
    void Read(cReadBlock &);
    void Write(cOutStream &) const;
    void Read(cInStream &);
};

cGUID cGUID::Generate() {
    cGUID g;
    g.a = cIRand();
    g.b = cIRand();
    return g;
}

void cGUID::Read(cReadBlock &rb) {
    cFileSystem::Read(*(void **)rb._data[0], this, 4);
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 4, 4);
}

void cGUID::Write(cOutStream &s) const {
    ((cOutStreamRef *)&s)->Write((unsigned int)a, 0x20, true);
    ((cOutStreamRef *)&s)->Write((unsigned int)b, 0x20, true);
}

void cGUID::Read(cInStream &s) {
    ((cInStreamRef *)&s)->Read((unsigned int &)a, 0x20, true);
    ((cInStreamRef *)&s)->Read((unsigned int &)b, 0x20, true);
}

// -----------------------------------------------------------------------------
// eMoviePlatform::soundbuf_checkDecodeEnd  @ 0x00013058 (eAll_psp.obj, 60B)
// -----------------------------------------------------------------------------

class eMoviePlatform {
public:
    int control_getCondition(void);
    int soundbuf_checkDecodeEnd(void);
};

int eMoviePlatform::soundbuf_checkDecodeEnd(void) {
    if (control_getCondition() & 8) return 1;
    return 0;
}
