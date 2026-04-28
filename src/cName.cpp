// cName — fixed-size name record (0x18 bytes). Layout inferred from
// cName::Read/Write below: a 0x14-byte inline char buffer at offset 0,
// followed by two 2-byte fields at 0x14 and 0x16.
//
// Functions matched here:
//   cName::Set(const char *, ...)        @ 0x00008afc  284B
//   cName::Write(cWriteBlock &) const    @ 0x00008c18   96B
//   cName::Read(cReadBlock &)            @ 0x00008c78  124B
//   cName::Write(cOutStream &) const     @ 0x00008cf4  152B
//   cName::Read(cInStream &)             @ 0x00008d8c  216B

#include "cOutStream.h"

typedef char *va_list;

class cFile;

class cReadBlock {
public:
    int _data[5];
};

class cFileSystem {
public:
    static void Read(void *handle, void *buf, unsigned int size);
};

class cWriteBlock {
public:
    void Write(int);
    void Write(int, const char *);
    void Write(short);
    void Write(unsigned short);
};

class cInStream {
public:
    void Read(int &, int, bool);
    void Read(unsigned int &, int, bool);
};

extern int cStrLength(const char *);
extern void cStrCopy(char *, const char *, int);
extern "C" int vsnprintf(char *, unsigned int, const char *, va_list);

struct cName {
    char           mName[0x14];   // 0x00 — 20-byte inline string
    short          mField14;      // 0x14
    unsigned short mField16;      // 0x16

    void Set(const char *, ...);
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
    void Write(cOutStream &) const;
    void Read(cInStream &);
};

// ── cName::Set ── @ 0x00008afc
void cName::Set(const char *fmt, ...) {
    char buf[0x28];
    register va_list args asm("a3");

    if (fmt == 0) {
        fmt = (const char *)0x36c878;
    }

    __asm__ volatile("lui $4, 0x0");
    __asm__ volatile("addiu $4, $4, 0x40");
    __asm__ volatile("andi $4, $4, 0xffff");
    __asm__ volatile("addu $7, $sp, $4");
    char *dst = buf;
    __asm__ volatile("addiu %0, $7, 0x8" : "=r"(args));
    vsnprintf(dst, 0x28, fmt, args);
    buf[0x27] = 0;

    int len = cStrLength(buf);
    int shortLen = len < 0x14;
    cStrCopy(mName, buf, 0x14);
    if (shortLen) {
        mField14 = (short)len;
    } else {
        mField14 = 0x13;
    }

    int sum = 0;
    int i = 0;
    int count = mField14;
    do {
        if (i >= count) {
            mName[i] = 0;
        } else {
            signed char c = mName[i];
            if (i & 1) {
                sum += c * 3;
            } else {
                sum += c * 7;
            }
        }
        i++;
    } while (i < 0x14);
    mField16 = sum;
}

// ── cName::Write ── @ 0x00008c18
void cName::Write(cWriteBlock &wb) const {
    wb.Write(0x14);
    wb.Write(0x14, mName);
    wb.Write(mField14);
    wb.Write(mField16);
}

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

// ── cName::Read ── @ 0x00008d8c
void cName::Read(cInStream &in) {
    char buf[0x14];
    unsigned int bits;

    {
        bool sign = true;
        in.Read(bits, 0x10, sign);
    }
    int len = bits & 0xffff;
    for (int i = 0; i < len; i++) {
        int value;
        bool sign = false;
        in.Read(value, 8, sign);
        int ch = (signed char)value;
        if (i < 0x14) {
            buf[i] = ch;
        }
    }

    int term = 0x13;
    if (len < 0x13) {
        term = len;
    }
    buf[term] = 0;
    Set(buf);
}
