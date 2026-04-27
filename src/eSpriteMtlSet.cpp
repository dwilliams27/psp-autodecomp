// eSpriteMtlSet.cpp — decompiled from eAll_psp.obj
// eSpriteMtlSet inherits from eMaterialSet.

class cFile;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class eMaterialSet {
public:
    void Write(cFile &) const;
};

class eSpriteMtlSet : public eMaterialSet {
public:
    void Write(cFile &) const;
};

// ── eSpriteMtlSet::Write(cFile &) const @ 0x00031AD0 ──
void eSpriteMtlSet::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const eMaterialSet *)this)->Write(file);
    wb.End();
}
