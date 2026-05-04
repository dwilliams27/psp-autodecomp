// ePathController — Write(cOutStream &) const  @ 0x0005b0a4
// ePathController — Read(cInStream &)          @ 0x0005b11c
// Obj: eAll_psp.obj

class cOutStream {
public:
    void Write(float, int, int, bool);
};

class cInStream {
public:
    void Read(float &, int, int, bool);
};

class cHandle {
public:
    int mId;
    void Write(cOutStream &) const;
    void Read(cInStream &);
};

class ePathController : public cHandle {
public:
    float mField4;
    float mField8;
    float mFieldC;

    void Write(cOutStream &) const;
    void Read(cInStream &);
};

#pragma control sched=1

void ePathController::Write(cOutStream &stream) const {
    cHandle::Write(stream);
    stream.Write(mField4, 10, 10, true);
    stream.Write(mField8, 10, 10, true);
    stream.Write(mFieldC, 10, 10, true);
}

void ePathController::Read(cInStream &stream) {
    cHandle::Read(stream);
    stream.Read(mField4, 10, 10, true);
    stream.Read(mField8, 10, 10, true);
    stream.Read(mFieldC, 10, 10, true);
}
