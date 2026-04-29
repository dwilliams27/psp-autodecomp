class cInStream {
public:
    void Read(unsigned int &, int, bool);
};

class cOutStream {
public:
    void Write(unsigned int, int, bool);
};

template <class T>
class cBaseArrayT;

class gcEntityAttack;
class gcEntityAttackSet;

class gcEntityAttackState {
public:
    gcEntityAttack *mAttack0;      // 0x00
    gcEntityAttack *mAttack1;      // 0x04
    gcEntityAttack *mAttack2;      // 0x08
    gcEntityAttack *mAttack3;      // 0x0C
    unsigned int mFlags;           // 0x10

    gcEntityAttack *ReadAttack(cInStream &, const cBaseArrayT<gcEntityAttackSet *> &);
    void WriteAttack(cOutStream &, const cBaseArrayT<gcEntityAttackSet *> &,
                     const gcEntityAttack *) const;

    void Read(cInStream &, const cBaseArrayT<gcEntityAttackSet *> &, bool);
    void Write(cOutStream &, const cBaseArrayT<gcEntityAttackSet *> &, bool) const;
};

void gcEntityAttackState::Read(cInStream &stream,
                               const cBaseArrayT<gcEntityAttackSet *> &attackSets,
                               bool readExtra) {
    mAttack0 = ReadAttack(stream, attackSets);
    mAttack1 = ReadAttack(stream, attackSets);
    if (readExtra) {
        mAttack2 = ReadAttack(stream, attackSets);
        mAttack3 = ReadAttack(stream, attackSets);
        stream.Read(mFlags, 2, false);
    }
}

void gcEntityAttackState::Write(cOutStream &stream,
                                const cBaseArrayT<gcEntityAttackSet *> &attackSets,
                                bool writeExtra) const {
    WriteAttack(stream, attackSets, mAttack0);
    WriteAttack(stream, attackSets, mAttack1);
    if (writeExtra) {
        gcEntityAttack *const *extraAttacks = &mAttack2;
        WriteAttack(stream, attackSets, extraAttacks[0]);
        WriteAttack(stream, attackSets, extraAttacks[1]);
        stream.Write(mFlags, 2, false);
    }
}
