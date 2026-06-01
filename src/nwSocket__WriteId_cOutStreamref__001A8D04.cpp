// nwSocket::WriteId(cOutStream &)
// Address: 0x001a8d04, Size: 112B
// Obj: nwAll_psp.obj
//
// Writes a 32-bit obfuscated connection id to the output stream: a fresh
// random word, then that word XOR-folded with a member salt and a magic
// constant. Both words are written as 32 raw bits.

#include "cOutStream.h"

extern "C" int cIRand(void);

class nwSocket {
public:
    char _pad[0x14];
    int mId;  // offset 0x14

    void WriteId(cOutStream &out);
};

void nwSocket::WriteId(cOutStream &out) {
    unsigned int r = (unsigned int)cIRand();
    unsigned int salt = (unsigned int)mId ^ 0x9E25C31A ^ ~r;
    out.Write(r, 0x20, true);
    out.Write(salt, 0x20, true);
}
