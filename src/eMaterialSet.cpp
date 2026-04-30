// eMaterialSet.cpp — decompiled from eAll_psp.obj

class cBase;
class cFile;
class cMemPool;
class cType;

class cObject {
public:
    char _pad[0x44];
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cHandle {
public:
    int mIndex;
    void Write(cWriteBlock &) const;
};

template <class T>
class cHandleT {
public:
    int mIndex;
};

template <class T>
class cArray {
public:
    void Read(class cReadBlock &);
};

class eMaterial;

class eMaterialSet : public cObject {
public:
    const cType *GetType(void) const;
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00040FF0;

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

void cFile_SetCurrentPos(void *, unsigned int);

// ── eMaterialSet::Write(cFile &) const @ 0x0002C30C ──
void eMaterialSet::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cObject::Write(file);

    cHandle *temp_a0 = *(cHandle **)((const char *)this + 0x44);
    int var_a1 = 0;
    if (temp_a0 != 0) {
        var_a1 = ((int *)temp_a0)[-1] & 0x3FFFFFFF;
    }
    wb.Write(var_a1);

    int var_s1 = 0;
    cHandle *temp_a0_2 = *(cHandle **)((const char *)this + 0x44);
    int var_s0 = 0;
    if (temp_a0_2 != 0) {
        var_s0 = ((int *)temp_a0_2)[-1] & 0x3FFFFFFF;
    }
    if (var_s1 < var_s0) {
        int var_s2 = 0;
        var_s2 = (int)temp_a0_2 + var_s2;
        do {
            ((cHandle *)var_s2)->Write(wb);
            var_s1 += 1;
            var_s2 += 4;
        } while (var_s1 < var_s0);
    }

    wb.End();
}

// ── eMaterialSet::Read(cFile &, cMemPool *) @ 0x0002C3D0 ──
int eMaterialSet::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 && cObject::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    ((cArray<cHandleT<eMaterial> > *)((char *)this + 0x44))->Read(rb);
    return result;
}

// ── eMaterialSet::GetType(void) const @ 0x001E15E0 ──
const cType *eMaterialSet::GetType(void) const {
    if (D_00040FF0 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                       (const char *)0x36CD7C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                                   &cNamed::New, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                               0, 0, 0, 0);
        }
        D_00040FF0 = cType::InitializeType(0, 0, 0x23, D_000385E4,
                                           0, (const char *)0x36CDB8,
                                           (const char *)0x36CDC8, 5);
    }
    return D_00040FF0;
}
