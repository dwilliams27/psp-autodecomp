// eHeightmapMtl.cpp — decompiled from eAll_psp.obj
// eHeightmapMtl inherits from eGeomMtl. Same Write pattern as
// eWeatherEffectMtl / eGeomTrailMtl: a 4-byte int field at offset 0x68 is serialized.

class cBase;
class cFile;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class eGeomMtl {
public:
    void Write(cFile &) const;
};

class eHeightmapMtl : public eGeomMtl {
public:
    void Write(cFile &) const;
};

#pragma control sched=1

// ── eHeightmapMtl::Write(cFile &) const @ 0x0007acd8 ──
void eHeightmapMtl::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const eGeomMtl *)this)->Write(file);
    wb.Write(*(int *)((char *)this + 0x68));
    wb.End();
}

#pragma control sched=2
