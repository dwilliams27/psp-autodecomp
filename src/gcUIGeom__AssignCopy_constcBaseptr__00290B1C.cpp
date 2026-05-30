// gcUIGeom::AssignCopy(const cBase *) @ 0x00290b1c, 420B, gcAll_psp.obj
// ODR-WARNING: split-TU local redeclaration for one gcUIGeom method.
// Mirrors verified-matched sibling eGeomTrail::AssignCopy (0x002119f4):
// inline dcast (nested cType::InitializeType chain + parent-chain walk),
// then forwards to gcUIGeom::operator= with the (possibly null) cast result.

class cBase;
class cMemPool;
class cType;

class cType {
public:
    char _pad[0x1C];
    cType *mParent;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

struct VTableSlot {
    short offset;
    short _pad;
    cType *(*getType)(void *);
};

class gcUIGeom {
public:
    void AssignCopy(const cBase *);
    gcUIGeom &operator=(const gcUIGeom &);
    static cBase *New(cMemPool *, cBase *);
};

extern cType *D_000385DC;
extern cType *D_00040FF4;
extern cType *D_000469C0;
extern cType *D_0009F58C;

// -- gcUIGeom::AssignCopy(const cBase *) @ 0x00290b1c --
void gcUIGeom::AssignCopy(const cBase *base) {
    const gcUIGeom *other = 0;

    if (base != 0) {
        if (D_0009F58C == 0) {
            if (D_000469C0 == 0) {
                if (D_00040FF4 == 0) {
                    if (D_000385DC == 0) {
                        D_000385DC = cType::InitializeType(
                            (const char *)0x36D894, (const char *)0x36D89C,
                            1, 0, 0, 0, 0, 0);
                    }
                    D_00040FF4 = cType::InitializeType(
                        0, 0, 0x16, D_000385DC, 0, 0, 0, 0);
                }
                D_000469C0 = cType::InitializeType(
                    0, 0, 0x17, D_00040FF4, 0, 0, 0, 0);
            }
            D_0009F58C = cType::InitializeType(
                0, 0, 0x82, D_000469C0, &gcUIGeom::New, 0, 0, 0);
        }

        VTableSlot *slot =
            (VTableSlot *)((char *)*(void **)((const char *)base + 4) + 8);
        cType *target = D_0009F58C;
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
                type = type->mParent;
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
            other = (const gcUIGeom *)base;
        }
    }

    this->operator=(*other);
}
