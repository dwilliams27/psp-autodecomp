// nwSocket::ReadId(cInStream &)
// Address: 0x001a8d74, Size: 124B
// Obj: nwAll_psp.obj
//
// Reverse of nwSocket::WriteId. Reads the fresh random word r and the
// obfuscated salt word from the input stream, recomputes the salt from the
// member id and returns whether they match (id validation).

class cInStream {
public:
    void Read(unsigned int &, int, bool);
};

class nwSocket {
public:
    char _pad[0x14];
    int mId;  // offset 0x14

    bool ReadId(cInStream &in);
};

bool nwSocket::ReadId(cInStream &in) {
    unsigned int r;
    unsigned int salt;
    in.Read(r, 0x20, true);
    in.Read(salt, 0x20, true);
    return ((unsigned int)mId ^ 0x9E25C31A ^ ~r) == salt;
}
