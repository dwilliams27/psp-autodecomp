// eTriangleShape.cpp - decompiled from eAll_psp.obj

class cBase;
class cMemPool;
class cType;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class eTriangleShape {
public:
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

static cType *type_cBase;
static cType *type_eShape;
static cType *type_eTriangleShape;

#pragma control sched=1

// eTriangleShape::GetType(void) const - 0x0020ad14
const cType *eTriangleShape::GetType(void) const {
    if (!type_eTriangleShape) {
        if (!type_eShape) {
            if (!type_cBase) {
                const char *name = (const char *)0x36CD74;
                const char *desc = (const char *)0x36CD7C;
                __asm__ volatile("" : "+r"(name), "+r"(desc));
                type_cBase = cType::InitializeType(name, desc, 1, 0, 0, 0, 0, 0);
            }
            type_eShape = cType::InitializeType(0, 0, 0x227, type_cBase, 0, 0, 0, 0);
        }
        __asm__ volatile("" ::: "memory");
        const cType *parentType = type_eShape;
        cBase *(*factory)(cMemPool *, cBase *) =
            (cBase *(*)(cMemPool *, cBase *))0x20AC98;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        type_eTriangleShape =
            cType::InitializeType(0, 0, 0x2D5, parentType, factory, 0, 0, 0);
    }
    return type_eTriangleShape;
}

#pragma control sched=2
