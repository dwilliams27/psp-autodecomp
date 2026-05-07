// ODR-WARNING: split-TU local redeclaration for one eNavMesh method.

class cBase;
class cFile;
class cMemPool;
class cWriteBlock;

class cObject {
public:
    void Write(cFile &) const;
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned char);
    void Write(int);
    void Write(int, const short *);
    void Write(int, const unsigned char *);
    void Write(int, const float *);
    void End(void);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

template <class T>
class cArrayBase {
public:
    void *mData;
};

class eNavTri;
class eNavVert;
class eNavPortal;

class eNavMesh : public cObject {
public:
    void Write(cFile &) const;
};

#pragma control sched=1

// -- eNavMesh::Write(cFile &) const @ 0x0005727c --
void eNavMesh::Write(cFile &file) const {
    int var_a0;
    int var_a1;
    int var_s0;
    int var_s1;
    int var_s1_2;
    int var_s1_3;
    int var_s1_4;
    int var_s2;
    int var_s2_2;
    int var_s3_2;
    void *temp_a0;
    void *temp_a0_2;
    void *temp_a0_3;
    void *temp_a0_4;
    void *temp_a0_5;
    void *temp_a1;
    void *temp_a1_2;
    void *temp_s3;

    cWriteBlock wb(file, 3);
    this->cObject::Write(file);

    temp_a0 = ((const cArrayBase<eNavTri> *)((const char *)this + 0x44))->mData;
    var_s1 = 0;
    if (temp_a0 != 0) {
        var_s1 = *(int *)((char *)temp_a0 - 4) & 0x3FFFFFFF;
    }
    wb.Write(var_s1);

    temp_a0_2 = ((const cArrayBase<eNavTri> *)((const char *)this + 0x44))->mData;
    var_s1_2 = 0;
    if (temp_a0_2 != 0) {
        var_s1_2 = *(int *)((char *)temp_a0_2 - 4) & 0x3FFFFFFF;
    }
    char *baseTris = (char *)temp_a0_2;
    var_s2 = 0;
    if (var_s2 < var_s1_2) {
        int offset = 0;
        unsigned char *tri = (unsigned char *)(baseTris + offset);
        do {
            wb.Write(3, tri);
            wb.Write(3, tri + 3);
            wb.Write(tri[6]);
            wb.Write(tri[7]);
            var_s2 += 1;
            tri += 8;
        } while (var_s2 < var_s1_2);
    }

    temp_a0_3 = ((const cArrayBase<eNavVert> *)((const char *)this + 0x48))->mData;
    var_s3_2 = 0;
    if (temp_a0_3 != 0) {
        var_s3_2 = *(int *)((char *)temp_a0_3 - 4) & 0x3FFFFFFF;
    }
    wb.Write(var_s3_2);

    temp_s3 = ((const cArrayBase<eNavVert> *)((const char *)this + 0x48))->mData;
    var_s1_3 = 0;
    if (temp_s3 != 0) {
        var_s1_3 = *(int *)((char *)temp_s3 - 4) & 0x3FFFFFFF;
    }
    char *baseVerts = (char *)temp_s3;
    var_s2_2 = 0;
    if (var_s2_2 < var_s1_3) {
        int offset = 0;
        short *vert = (short *)(baseVerts + offset);
        do {
            wb.Write(3, vert);
            var_s2_2 += 1;
            vert = (short *)((char *)vert + 6);
        } while (var_s2_2 < var_s1_3);
    }

    temp_a1 = ((const cArrayBase<unsigned char> *)((const char *)this + 0x4C))->mData;
    var_a0 = 0;
    if (temp_a1 != 0) {
        var_a0 = *(int *)((char *)temp_a1 - 4) & 0x3FFFFFFF;
    }
    wb.Write(var_a0);

    temp_a0_4 = ((const cArrayBase<unsigned char> *)((const char *)this + 0x4C))->mData;
    var_a1 = 0;
    if (temp_a0_4 != 0) {
        var_a1 = *(int *)((char *)temp_a0_4 - 4) & 0x3FFFFFFF;
    }
    wb.Write(var_a1, (const unsigned char *)temp_a0_4);

    wb.Write(3, (const float *)((const char *)this + 0x50));
    wb.Write(3, (const float *)((const char *)this + 0x60));
    wb.Write(3, (const float *)((const char *)this + 0x70));

    temp_a1_2 = ((const cArrayBase<eNavPortal> *)((const char *)this + 0x80))->mData;
    var_a0 = 0;
    if (temp_a1_2 != 0) {
        var_a0 = *(int *)((char *)temp_a1_2 - 4) & 0x3FFFFFFF;
    }
    wb.Write(var_a0);

    temp_a0_5 = ((const cArrayBase<eNavPortal> *)((const char *)this + 0x80))->mData;
    var_s0 = 0;
    if (temp_a0_5 != 0) {
        var_s0 = *(int *)((char *)temp_a0_5 - 4) & 0x3FFFFFFF;
    }
    char *basePortals = (char *)temp_a0_5;
    var_s1_4 = 0;
    if (var_s1_4 < var_s0) {
        int offset = 0;
        unsigned char *portal = (unsigned char *)(basePortals + offset);
        do {
            ((const cHandle *)portal)->Write(wb);
            wb.Write(portal[4]);
            wb.Write(portal[5]);
            wb.Write(portal[6]);
            var_s1_4 += 1;
            portal += 8;
        } while (var_s1_4 < var_s0);
    }

    wb.End();
}
