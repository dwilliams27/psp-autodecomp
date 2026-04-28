// cHandle — opaque handle into the global cManager object table.
//
// Functions matched here:
//   cHandle::Write(cOutStream &) const  @ 0x000092f0  128B
//   cHandle::Read(cInStream &)          @ 0x00009370  124B

class cInStream;
class cOutStream;

class cObjectKey {
public:
    unsigned int mKey;
    void Read(cInStream &);
    void Write(cOutStream &) const;
};

class cObject {
public:
    char _pad0[0x28];
    unsigned short mFlags;   // 0x28
    char _pad2a[6];
    int mIndex;              // 0x30
};

class cObjectFull {
public:
    char _pad0[0x20];
    unsigned int mWord20;    // 0x20
    unsigned int mWord24;    // 0x24
    char _pad28[8];
    int mIndex;              // 0x30
};

class cManager {
public:
    static cObject *Find(const cObjectKey &);
};

class cHandle : public cObjectKey {
public:
    void Read(cInStream &);
    void Write(cOutStream &) const;
};

extern void *D_00038890[];   // global handle-pointer table

// 0x000092f0 — encrypt-and-write the handle's key to a cOutStream.
void cHandle::Write(cOutStream &s) const {
    cObjectKey out;
    unsigned int idx = mKey;
    void *obj;
    if (idx == 0) {
        obj = 0;
    } else {
        void *cand = *(D_00038890 + (idx & 0xFFFF));
        obj = 0;
        if (cand != 0 && *(int *)((char *)cand + 0x30) == (int)idx) {
            obj = cand;
        }
    }
    unsigned int key = 0;
    if (obj != 0) {
        key = ((cObjectFull *)obj)->mWord20 ^ ((cObjectFull *)obj)->mWord24;
    }
    out.mKey = key;
    out.Write(s);
}

// 0x00009370 — read a key from cInStream, look up via cManager::Find,
// and store the resolved index back into the handle's mKey.
void cHandle::Read(cInStream &s) {
    cObjectKey result;
    cObjectKey input;
    input.mKey = 0;
    input.Read(s);
    cObject *obj = cManager::Find(input);
    if (obj != 0) {
        if (!(((obj->mFlags & 0x10) != 0) & 0xFF)) {
            obj = 0;
        }
    }
    int idx = 0;
    if (obj != 0) {
        idx = obj->mIndex;
    }
    result.mKey = idx;
    *(cObjectKey *)this = result;
}
