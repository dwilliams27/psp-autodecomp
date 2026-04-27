// eBipedControllerTemplate.cpp — decompiled from eAll_psp.obj.
// Parent: ePhysicsControllerTemplate.
//
// Functions:
//   0x00062858 eBipedControllerTemplate::Write(cFile &) const  100B

class cBase;
class cFile;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(float);
    void End(void);
};

class ePhysicsControllerTemplate {
public:
    void Write(cFile &) const;
};

class eBipedControllerTemplate : public ePhysicsControllerTemplate {
public:
    char _pad[0x2C - 0x00];
    float mField2C;  // 0x2C
    float mField30;  // 0x30

    void Write(cFile &) const;
};

#pragma control sched=1

// ── eBipedControllerTemplate::Write @ 0x00062858 ──
void eBipedControllerTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    ePhysicsControllerTemplate::Write(file);
    wb.Write(mField30);
    wb.Write(mField2C);
    wb.End();
}
