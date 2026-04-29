#include "eCompoundShape.h"
#include "eCollision.h"

inline void *operator new(unsigned int, void *p) { return p; }

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

extern cType *D_000385DC;
extern cType *D_00040FE4;
extern cType *D_00046BE4;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cBaseArray {
public:
    void Write(cWriteBlock &) const;
};

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

// ── eCompoundShape::eCompoundShape(cBase *) @ 0x00072dd4 ──
eCompoundShape::eCompoundShape(cBase *b) : eShape(b) {
    *(void **)((char *)this + 4) = (void *)0x3839F0;
    mSubShapes = 0;
    mSelf = this;
}

// ── eCompoundShape::Write(cFile &) const @ 0x00072ae0 ──
void eCompoundShape::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    eShape::Write(file);
    ((const cBaseArray *)((const char *)this + 0x80))->Write(wb);
    wb.End();
}

// ── eCompoundShape::New(cMemPool *, cBase *) static @ 0x0020b36c ──
eCompoundShape *eCompoundShape::New(cMemPool *pool, cBase *parent) {
    eCompoundShape *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eCompoundShape *obj = (eCompoundShape *)rec->fn(base, 0xB0, 0x10, 0, 0);
    if (obj != 0) {
        new (obj) eCompoundShape(parent);
        result = obj;
    }
    return result;
}

// ── eCompoundShape::GetType(void) const @ 0x0020b3e8 ──
const cType *eCompoundShape::GetType(void) const {
    if (D_00046BE4 == 0) {
        if (D_00040FE4 == 0) {
            if (D_000385DC == 0) {
                const char *name = (const char *)0x36CD74;
                const char *desc = (const char *)0x36CD7C;
                __asm__ volatile("" : "+r"(name), "+r"(desc));
                D_000385DC = cType::InitializeType(
                    name, desc, 1, 0, 0, 0, 0, 0);
            }
            D_00040FE4 = cType::InitializeType(
                0, 0, 0x227, D_000385DC, 0, 0, 0, 0);
        }
        __asm__ volatile("" ::: "memory");
        const cType *parentType = D_00040FE4;
        cBase *(*factory)(cMemPool *, cBase *) =
            (cBase *(*)(cMemPool *, cBase *))0x20B36C;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046BE4 = cType::InitializeType(
            0, 0, 0x22D, parentType, factory, 0, 0, 0);
    }
    return D_00046BE4;
}

// ── eCompoundShape::Collide(const eSphereShape *, ...) @ 0x00073820 ──
int eCompoundShape::Collide(const eSphereShape *shape, int a, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    if (eCollision::SphereCompound(*shape, *this, a, ocs2, ocs1, info) != 0) {
        int i = 0;
        if (i < *(int *)((char *)info + 0x14)) {
            char *p = (char *)info + 0x20;
            do {
                __asm__ volatile(
                    "lv.q C120, 0(%0)\n"
                    "vneg.t C120, C120\n"
                    "sv.q C120, 0(%0)\n"
                    :: "r"(p) : "memory"
                );
                i++;
                p += 0x40;
            } while (i < *(int *)((char *)info + 0x14));
        }
        return 1;
    }
    return 0;
}

// ── eCompoundShape::Collide(const eCompoundShape *, ...) @ 0x00073ac8 ──
int eCompoundShape::Collide(const eCompoundShape *other, int a, int b, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::CompoundCompound(*this, *other, a, b, ocs1, ocs2, info);
}

// ── eCompoundShape::Collide(const eMeshShape *, ...) @ 0x00073ae4 ──
int eCompoundShape::Collide(const eMeshShape *other, int a, int b, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::CompoundMesh(*this, *other, a, b, ocs1, ocs2, info);
}

// ── eCompoundShape::Collide(const eHeightmapShape *, ...) @ 0x00073b00 ──
int eCompoundShape::Collide(const eHeightmapShape *other, int a, int b, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::CompoundHeightmap(*this, *other, a, b, ocs1, ocs2, info);
}
