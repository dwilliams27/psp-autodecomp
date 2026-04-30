class cBase;

class cObject {
public:
    cObject(cBase *);
};

class gcEntityTemplate : public cObject {
public:
    char _base[0x44];
    int m44;
    int m48;
    int m4C;
    gcEntityTemplate *m50;
    int m54;
    gcEntityTemplate *m58;
    int m5C;
    gcEntityTemplate *m60;
    int m64;
    gcEntityTemplate *m68;
    int m6C;
    gcEntityTemplate *m70;
    bool m74;
    bool m75;
    char _pad76[2];
    int m78;
    gcEntityTemplate *m7C;
    int m80;
    gcEntityTemplate *m84;
    int m88;
    int m8C;
    int m90;
    int m94;
    bool m98;
    char _pad99[0xAC - 0x99];
    short mAC;
    short mAE;
    bool mB0;
    char _padB1[0xC4 - 0xB1];
    short mC4;
    short mC6;
    int mC8;
    int mCC;

    gcEntityTemplate(cBase *);
};

// ============================================================
// 0x00126650 - gcEntityTemplate(cBase *)
// ============================================================
gcEntityTemplate::gcEntityTemplate(cBase *parent) : cObject(parent) {
    *(void **)((char *)this + 4) = (void *)0x389468;
    m44 = 0;
    m48 = 0;
    m4C = 0;
    m50 = this;
    m54 = 0;
    m58 = this;
    m5C = 0;
    m60 = this;
    m64 = 0;
    m68 = this;
    m6C = 0;
    m70 = this;
    m74 = 0;
    m75 = 0;
    m78 = 0;
    m7C = this;
    m80 = 0;
    m84 = this;
    m88 = 0;
    m8C = 0;
    m90 = 0;
    m94 = 0;
    mAC = 0;
    mAE = 0;
    m98 = 0;
    mC4 = 0;
    mC6 = 0;
    mB0 = 0;
    mC8 = 0;
    mCC = 1;
}
