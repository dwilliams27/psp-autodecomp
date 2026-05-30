#pragma control sched=1

class cFile;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class eDynamicGeom {
public:
    void Write(cFile &) const;
};

class eDynamicLight : public eDynamicGeom {
public:
    void Write(cFile &) const;
};

void eDynamicLight::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    eDynamicGeom::Write(file);
    wb.End();
}
