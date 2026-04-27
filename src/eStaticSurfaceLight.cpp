#include "eStaticLight.h"

class cFile;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int, const float *);
    void End(void);
};

class eStaticSurfaceLight : public eStaticLight {
public:
    void Write(cFile &) const;
};

#pragma control sched=1
// eStaticSurfaceLight::Write(cFile &) const — 0x0005ff54
void eStaticSurfaceLight::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const eStaticLight *)this)->Write(file);
    wb.Write(2, (const float *)((const char *)this + 0x90));
    wb.End();
}
