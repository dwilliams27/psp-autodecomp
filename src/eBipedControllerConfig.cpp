// eBipedControllerConfig.cpp — decompiled from eAll_psp.obj
// eBipedControllerConfig inherits from ePhysicsControllerConfig.

class cBase;
class cFile;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(float);
    void End(void);
};

class ePhysicsControllerConfig {
public:
    void Write(cFile &) const;
};

class eBipedControllerConfig : public ePhysicsControllerConfig {
public:
    void Write(cFile &) const;
};

#pragma control sched=1

// ── eBipedControllerConfig::Write(cFile &) const @ 0x00062610 ──
void eBipedControllerConfig::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    ((const ePhysicsControllerConfig *)this)->Write(file);
    wb.Write(*(float *)((char *)this + 0x20));
    wb.End();
}

#pragma control sched=2
