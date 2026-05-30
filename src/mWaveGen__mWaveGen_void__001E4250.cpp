// mWaveGen::mWaveGen(void) @ 0x001e4250  symbol __0oImWaveGenctv
// Default constructor: float members + one int.

#pragma control sched=1

class mWaveGen {
public:
    float m0;
    float m4;
    float m8;
    float mC;
    int   m10;
    mWaveGen();
};

mWaveGen::mWaveGen() {
    m10 = 0;
    m0  = 0.0f;
    m4  = 1.0f;
    m8  = 0.0f;
    mC  = 1.0f;
}
