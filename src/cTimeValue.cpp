// -----------------------------------------------------------------------------
// cTimeValue::Read(cInStream &)  @ 0x0000d444 (cAll_psp.obj, 44B)
// -----------------------------------------------------------------------------

class cInStream;

class cInStreamRef {
public:
    void Read(int &, int, bool);
};

class cTimeValue {
public:
    int mTime;
    void Read(cInStream &);
};

void cTimeValue::Read(cInStream &s) {
    ((cInStreamRef *)&s)->Read(mTime, 0x20, true);
}
