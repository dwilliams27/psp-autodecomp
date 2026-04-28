// cName — fixed-size name record (0x18 bytes). Layout inferred from
// cName::Read/Write below: a 0x14-byte inline char buffer at offset 0,
// followed by two 2-byte fields at 0x14 and 0x16.
//
// Functions matched here:
//   cName::Read(cReadBlock &)            @ 0x00008c78  124B
//   cName::Write(cOutStream &) const     @ 0x00008cf4  152B

#include "cOutStream.h"

class cFile;

class cReadBlock {
public:
    int _data[5];
};

class cFileSystem {
public:
    static void Read(void *handle, void *buf, unsigned int size);
};

extern int cStrLength(const char *);

struct cName {
    char           mName[0x14];   // 0x00 — 20-byte inline string
    unsigned short mField14;      // 0x14
    unsigned short mField16;      // 0x16

    void Read(cReadBlock &);
    void Write(cOutStream &) const;
};

// ── cName::Read ── @ 0x00008c78
void cName::Read(cReadBlock &rb) {
    int header;
    cFileSystem::Read(*(void **)rb._data[0], &header, 4);
    cFileSystem::Read(*(void **)rb._data[0], (char *)this, 0x14);
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 0x14, 2);
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 0x16, 2);
}

// ── cName::Write ── @ 0x00008cf4
void cName::Write(cOutStream &out) const {
    int len = cStrLength(mName) & 0xFFFF;
    {
        bool sign2 = true;
        out.Write((unsigned int)(len & 0xFFFF), 0x10, sign2);
    }
    for (int i = 0; i < len; i++) {
        bool sign = false;
        signed char v = mName[i];
        int iv = v;
        out.Write((signed char)iv, 8, sign);
    }
}
