// eDynamicGeomTemplate.cpp — decompiled from eAll_psp.obj
// eDynamicGeomTemplate inherits from eGeomTemplate.

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(float);
    void End(void);
};

class eGeomTemplate {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class eDynamicGeomTemplate : public eGeomTemplate {
public:
    void Write(cFile &) const;
};

#pragma control sched=1
// ── eDynamicGeomTemplate::Write(cFile &) const @ 0x0004B0F8 ──
void eDynamicGeomTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const eGeomTemplate *)this)->Write(file);
    wb.Write(*(float *)((char *)this + 0x44));
    wb.End();
}
