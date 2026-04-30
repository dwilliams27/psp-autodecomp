extern "C" void *memset(void *, int, unsigned int);

#pragma control sched=1

class eDynamicMeshVisSkinTriList {
public:
    int mField0;
    int mField4;
    int mField8;
    int mFieldC;
    short mField10;
    short mField12;
    short mField14;
    short mPad16;
    int mField18;
    int mField1C;
    float mField20;

    eDynamicMeshVisSkinTriList(void);
};

eDynamicMeshVisSkinTriList::eDynamicMeshVisSkinTriList(void) {
    mField8 = 0;
    mFieldC = 0;
    mField10 = 0;
    mField12 = 0;
    mField14 = 0;
    mField18 = 0;
    mField1C = 0;
    __asm__ volatile("" ::: "memory");
    mField20 = 0.0f;
    memset(this, 0, 8);
}
