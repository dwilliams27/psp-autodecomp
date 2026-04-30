// eDynamicMeshExtrudedShadowVertex.cpp -- decompiled from eAll_psp.obj

class cWriteBlock {
public:
    void Write(float);
    void Write(unsigned char);
};

class cReadBlock {
public:
    int _data[4];
};

class cFileSystem {
public:
    static int Read(void *, void *, unsigned int);
};

class eDynamicMeshExtrudedShadowVertex {
public:
    float mPos[3];
    unsigned char mA;
    unsigned char mB;
    unsigned char mC;
    unsigned char mD;

    void Read(cReadBlock &);
    void Write(cWriteBlock &) const;
};

#pragma control sched=1

void eDynamicMeshExtrudedShadowVertex::Read(cReadBlock &rb) {
    cReadBlock *in = &rb;
    eDynamicMeshExtrudedShadowVertex *self = this;
    __asm__ volatile("" : "+r"(in), "+r"(self));
    cFileSystem::Read(*(void **)in->_data[0], self->mPos + 0, 4);
    cFileSystem::Read(*(void **)in->_data[0], self->mPos + 1, 4);
    cFileSystem::Read(*(void **)in->_data[0], self->mPos + 2, 4);
    cFileSystem::Read(*(void **)in->_data[0], &self->mA, 1);
    cFileSystem::Read(*(void **)in->_data[0], &self->mB, 1);
    cFileSystem::Read(*(void **)in->_data[0], &self->mC, 1);
    cFileSystem::Read(*(void **)in->_data[0], &self->mD, 1);
}

void eDynamicMeshExtrudedShadowVertex::Write(cWriteBlock &wb) const {
    cWriteBlock *out = &wb;
    const eDynamicMeshExtrudedShadowVertex *self = this;
    __asm__ volatile("" : "+r"(out), "+r"(self));
    out->Write(self->mPos[0]);
    out->Write(self->mPos[1]);
    out->Write(self->mPos[2]);
    out->Write(self->mA);
    out->Write(self->mB);
    out->Write(self->mC);
    out->Write(self->mD);
}
#pragma control sched=2
