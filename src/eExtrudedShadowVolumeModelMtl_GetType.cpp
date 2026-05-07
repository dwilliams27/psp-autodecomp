// ODR-WARNING: this TU intentionally redeclares eExtrudedShadowVolumeModelMtl
// with only the method needed here. The main class declaration in
// eTextureMap.h does not currently list GetType, and adding it there can
// perturb sibling TUs.

class cBase;
class cMemPool;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class eExtrudedShadowVolumeModelMtl {
public:
    const cType *GetType(void) const;
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00040FEC;
extern cType *D_00040FF8;
extern cType *D_00046B28;
extern cType *D_00046C98;
extern cType *D_00046C9C;

#pragma control sched=1

const cType *eExtrudedShadowVolumeModelMtl::GetType(void) const {
    if (D_00046C9C == 0) {
        if (D_00046C98 == 0) {
            if (D_00046B28 == 0) {
                if (D_00040FF8 == 0) {
                    if (D_00040FEC == 0) {
                        if (D_000385E4 == 0) {
                            if (D_000385E0 == 0) {
                                if (D_000385DC == 0) {
                                    const char *name = (const char *)0x36CD74;
                                    const char *desc = (const char *)0x36CD7C;
                                    __asm__ volatile("" : "+r"(name), "+r"(desc));
                                    D_000385DC = cType::InitializeType(
                                        name, desc, 1, 0, 0, 0, 0, 0);
                                }
                                const cType *parentType = D_000385DC;
                                cBase *(*factory)(cMemPool *, cBase *) =
                                    (cBase *(*)(cMemPool *, cBase *))0x1C3C58;
                                __asm__ volatile("" : "+r"(parentType), "+r"(factory));
                                D_000385E0 = cType::InitializeType(
                                    0, 0, 2, parentType, factory, 0, 0, 0);
                            }
                            D_000385E4 = cType::InitializeType(
                                0, 0, 3, D_000385E0, 0, 0, 0, 0);
                        }
                        const cType *parentType = D_000385E4;
                        const char *kindName = (const char *)0x36CDCC;
                        const char *kindDesc = (const char *)0x36CDD8;
                        __asm__ volatile("" : "+r"(parentType), "+r"(kindName), "+r"(kindDesc));
                        D_00040FEC = cType::InitializeType(
                            0, 0, 0x10, parentType, 0, kindName, kindDesc, 5);
                    }
                    D_00040FF8 = cType::InitializeType(0, 0, 0x12, D_00040FEC,
                                                       0, 0, 0, 0);
                }
                D_00046B28 = cType::InitializeType(0, 0, 0x13, D_00040FF8,
                                                   0, 0, 0, 0);
            }
            const cType *parentType = D_00046B28;
            cBase *(*factory)(cMemPool *, cBase *) =
                (cBase *(*)(cMemPool *, cBase *))0x21A8C4;
            __asm__ volatile("" : "+r"(parentType), "+r"(factory));
            D_00046C98 = cType::InitializeType(0, 0, 0x213, parentType, factory,
                                               0, 0, 0);
        }
        const cType *parentType = D_00046C98;
        cBase *(*factory)(cMemPool *, cBase *) =
            (cBase *(*)(cMemPool *, cBase *))0x21ABC4;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046C9C = cType::InitializeType(0, 0, 0x5C, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046C9C;
}
