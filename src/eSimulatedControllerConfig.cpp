class cBase;
class cFile;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned int);
    void Write(float);
    void End(void);
};

class ePhysicsControllerConfig {
public:
    void *mParent;
    void *mVtable;
    unsigned char mField08;
    char mPad09[0x13];
    short mField1C;
    short mField1E;

    ePhysicsControllerConfig(cBase *);
    void Write(cFile &) const;
};

class eSimulatedControllerConfig : public ePhysicsControllerConfig {
public:
    float mField20;
    float mField24;
    float mField28;
    float mField2C;
    float mField30;
    float mField34;
    float mField38;
    float mField3C;
    unsigned int mField40;

    eSimulatedControllerConfig(cBase *);
    void Write(cFile &) const;
};

#pragma control sched=1

void eSimulatedControllerConfig::Write(cFile &file) const {
    cWriteBlock wb(file, 7);
    ((const ePhysicsControllerConfig *)this)->Write(file);
    wb.Write(mField20);
    wb.Write(mField24);
    wb.Write(mField28);
    wb.Write(mField2C);
    wb.Write(mField30);
    wb.Write(mField34);
    wb.Write(mField38);
    wb.Write(mField3C);
    wb.Write(mField40);
    wb.End();
}

eSimulatedControllerConfig::eSimulatedControllerConfig(cBase *parent)
    : ePhysicsControllerConfig(parent) {
    mVtable = (void *)0x383E98;
    mField20 = 1.0f;
    mField24 = 1.0f;
    mField28 = 1.0f;
    __asm__ volatile("" ::: "memory");
    mField2C = 0.98f;
    mField30 = 0.96f;
    mField34 = 0.25f;
    mField38 = 0.25f;
    __asm__ volatile("" ::: "memory");
    mField3C = 1.3f;
    mField40 = 0;
}
