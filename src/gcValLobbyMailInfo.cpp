// gcValLobbyMailInfo — decompiled from gcAll_psp.obj
// Methods in this file:
//   0x0034a140  Write(cFile &) const
//
// Class layout (partial):
//   [0x00] gcValue base (parent + vtable)
//   [0x08] embedded gcDesiredValue
//   [0x0C] mField (int written between base and gcDesiredValue::Write)

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcValue {
public:
    cBase *mParent;
    void *mVtable;
    void Write(cFile &) const;
};

class gcValLobbyMailInfo : public gcValue {
public:
    int mField8;   // 0x08 — start of embedded gcDesiredValue
    int mFieldC;   // 0x0C — int field written by wb.Write before gcDesiredValue::Write

    void Write(cFile &) const;
};

// ── gcValLobbyMailInfo::Write(cFile &) const @ 0x0034a140 ──
void gcValLobbyMailInfo::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcValue *)this)->Write(file);
    wb.Write(this->mFieldC);
    ((gcDesiredValue *)((char *)this + 8))->Write(wb);
    wb.End();
}
