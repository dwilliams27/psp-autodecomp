// eSurfacePropertyTable - eAll_psp.obj
//   0x001f6f28 AssignCopy(const cBase *)

#include "cBase.h"
#include "cObject.h"

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cFile;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(float);
    void End(void);
};

template <class T> T *dcast(const cBase *);

class cBaseArray {
public:
    void *mData;
    void *mOwner;

    cBaseArray &operator=(const cBaseArray &);
    void Write(cWriteBlock &) const;
};

template <class T>
class cArrayBase {
public:
    void *mData;

    cArrayBase &operator=(const cArrayBase &);
};

class eSurfacePropertyPair;

class eSurfacePropertyTable : public cObject {
public:
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00046A20;

#pragma control sched=1
void eSurfacePropertyTable::AssignCopy(const cBase *base) {
    eSurfacePropertyTable *other = dcast<eSurfacePropertyTable>(base);
    cObject::operator=(*other);
    ((cBaseArray *)((char *)this + 0x44))->operator=(
        *(const cBaseArray *)((char *)other + 0x44));
    ((cArrayBase<eSurfacePropertyPair> *)((char *)this + 0x4C))->operator=(
        *(const cArrayBase<eSurfacePropertyPair> *)((char *)other + 0x4C));
}

const cType *eSurfacePropertyTable::GetType(void) const {
    if (D_00046A20 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    const char *name = (const char *)0x36CD74;
                    const char *desc = (const char *)0x36CD7C;
                    __asm__ volatile("" : "+r"(name), "+r"(desc));
                    D_000385DC = cType::InitializeType(name, desc, 1, 0, 0, 0,
                                                       0, 0);
                }
                const cType *parentType = D_000385DC;
                cBase *(*factory)(cMemPool *, cBase *) = &cNamed::New;
                __asm__ volatile("" : "+r"(parentType), "+r"(factory));
                D_000385E0 = cType::InitializeType(0, 0, 2, parentType, factory,
                                                   0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0, 0, 0, 0,
                                               0);
        }
        const cType *parentType = D_000385E4;
        cBase *(*factory)(cMemPool *, cBase *) = &eSurfacePropertyTable::New;
        const char *kindName = (const char *)0x36CE78;
        const char *kindDesc = (const char *)0x36CE90;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory), "+r"(kindName),
                         "+r"(kindDesc));
        D_00046A20 = cType::InitializeType(0, 0, 0x2B8, parentType, factory,
                                           kindName, kindDesc, 5);
    }
    return D_00046A20;
}

#pragma control sched=2

void eSurfacePropertyTable::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const cObject *)this)->Write(file);
    ((const cBaseArray *)((const char *)this + 0x44))->Write(wb);

    int *temp_a0 = (int *)((cArrayBase<int *> *)((char *)this + 0x4C))->mData;
    int var_s1 = 0;
    if (temp_a0 != 0) {
        var_s1 = temp_a0[-1] & 0x3FFFFFFF;
    }
    wb.Write(var_s1);

    int *temp_s0 = (int *)((cArrayBase<int *> *)((char *)this + 0x4C))->mData;
    int var_s1_2 = 0;
    if (temp_s0 != 0) {
        var_s1_2 = temp_s0[-1] & 0x3FFFFFFF;
    }

    int var_s2 = 0;
    if (var_s2 < var_s1_2) {
        int *var_s3_base = temp_s0;
        int var_s3_offset = 0;
        __asm__ volatile("" : "+r"(var_s3_base), "+r"(var_s3_offset));
        int *var_s3 = var_s3_base + var_s3_offset;
        do {
            int *table0 = var_s3 + 0;
            __asm__ volatile("" : "+r"(table0));
            int *temp_a1 = *(int **)table0;
            int var_a0 = 0;
            if (temp_a1 != 0) {
                var_a0 = temp_a1[-1] & 0x3FFFFFFF;
            }
            int *table1 = table0 + 0;
            __asm__ volatile("" : "+r"(table1));
            wb.Write(var_a0);

            int *temp_a0_2 = *(int **)table1;
            int var_s4 = 0;
            if (temp_a0_2 != 0) {
                var_s4 = temp_a0_2[-1] & 0x3FFFFFFF;
            }

            int var_s5 = 0;
            if (var_s5 < var_s4) {
                float *var_s6_base = (float *)temp_a0_2;
                int var_s6_offset = 0;
                __asm__ volatile("" : "+r"(var_s6_base), "+r"(var_s6_offset));
                float *var_s6 =
                    (float *)((char *)var_s6_base + var_s6_offset);
                do {
                    wb.Write(var_s6[0]);
                    wb.Write(var_s6[1]);
                    wb.Write(var_s6[2]);
                    wb.Write(var_s6[3]);
                    var_s5 += 1;
                    var_s6 = (float *)((char *)var_s6 + 0x10);
                } while (var_s5 < var_s4);
            }

            var_s2 += 1;
            var_s3 += 1;
        } while (var_s2 < var_s1_2);
    }

    wb.End();
}
