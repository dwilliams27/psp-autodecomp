// ODR-WARNING: this TU intentionally redeclares eTwoPassModelMtl with a
// minimal member list because include/eTwoPassModelMtl.h does not yet declare
// AssignCopy. Keep this split from src/eTwoPassModelMtl.cpp so existing
// matches in the canonical TU are not perturbed.

class cBase;
class cMemPool;
class cType;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class eTwoPassModelMtl {
public:
    static eTwoPassModelMtl *New(cMemPool *, cBase *);
    eTwoPassModelMtl &operator=(const eTwoPassModelMtl &);
    void AssignCopy(const cBase *);
};

struct cTypeNode {
    char pad[0x1C];
    cType *parent;
};

struct VTableSlot {
    short offset;
    short _pad;
    cType *(*getType)(void *);
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00040FEC;
extern cType *D_00040FF8;
extern cType *D_00046B28;
extern cType *D_00046CB8;

#pragma control sched=1

void eTwoPassModelMtl::AssignCopy(const cBase *base) {
    const eTwoPassModelMtl *other = 0;

    if (base != 0) {
        if (D_00046CB8 == 0) {
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
                    D_00040FF8 = cType::InitializeType(
                        0, 0, 0x12, D_00040FEC, 0, 0, 0, 0);
                }
                D_00046B28 = cType::InitializeType(
                    0, 0, 0x13, D_00040FF8, 0, 0, 0, 0);
            }
            const cType *parentType = D_00046B28;
            cBase *(*factory)(cMemPool *, cBase *) =
                (cBase *(*)(cMemPool *, cBase *))0x21C55C;
            __asm__ volatile("" : "+r"(parentType), "+r"(factory));
            D_00046CB8 = cType::InitializeType(
                0, 0, 0x49, parentType, factory, 0, 0, 0);
        }

        cType *target = D_00046CB8;
        __asm__ volatile("" : "+r"(target));
        void *classDesc = *(void **)((const char *)base + 4);
        VTableSlot *slot = (VTableSlot *)((char *)classDesc + 8);
        short offset = slot->offset;
        cType *type = slot->getType((void *)((const char *)base + offset));
        int ok;

        if (target != 0) {
            goto have_target;
        }
        ok = 0;
        goto cast_done;

have_target:
        if (type != 0) {
loop:
            if (type == target) {
                ok = 1;
            } else {
                type = ((cTypeNode *)type)->parent;
                if (type != 0) {
                    goto loop;
                }
                goto invalid;
            }
        } else {
invalid:
            ok = 0;
        }

cast_done:
        if (ok != 0) {
            other = (const eTwoPassModelMtl *)base;
        }
    }

    *this = *other;
}
