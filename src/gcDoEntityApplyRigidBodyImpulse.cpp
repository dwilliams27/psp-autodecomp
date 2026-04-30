class cBase;
class cFile;
class cMemPool;
class cType;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    cFile *_file;
    int _pos;

    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cName {
public:
    void Write(cWriteBlock &) const;
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcExpression {
};

class gcAction : public gcExpression {
public:
    cBase *mOwner;
    void *mVTable;
    unsigned int mNext;

    void Write(cFile &) const;
};

class gcDoEntityApplyRigidBodyImpulse : public gcAction {
public:
    gcDoEntityApplyRigidBodyImpulse(cBase *);

    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    gcDoEntityApplyRigidBodyImpulse &
    operator=(const gcDoEntityApplyRigidBodyImpulse &);
};

struct WriteRec {
    short offset;
    short _pad;
    void (*fn)(void *, cFile *);
};

struct cTypeNode {
    char pad[0x1C];
    cTypeNode *parent;
};

struct VTableSlot {
    short offset;
    short _pad;
    const cType *(*getType)(void *);
};

extern cType *D_000385D4;
extern cType *D_000385D8;
extern cType *D_000385DC;
extern cType *D_0009F5E8;

extern "C" void gcAction_ctor(void *, cBase *)
    __asm__("gcAction__gcAction_cBaseptr__0012F4C8");
extern "C" void gcDesiredObject_ctor(void *, cBase *)
    __asm__("gcDesiredObject__gcDesiredObject_cBaseptr__0011B6F4");
extern "C" void gcDesiredEntityHelper_ctor(void *, int, int, int)
    __asm__("gcDesiredEntityHelper__gcDesiredEntityHelper_gcDesiredEntityHelper__gcPrimary_gcDesiredEntityHelper__gcRelationship_gcDesiredEntityHelper__gcRelationship__0011B714");

extern char D_00000338[];
extern char D_000020A8[];

gcDoEntityApplyRigidBodyImpulse::gcDoEntityApplyRigidBodyImpulse(cBase *parent) {
    gcAction_ctor(this, parent);

    *(void **)((char *)this + 0x04) = D_000020A8;

    int one = 1;
    *(int *)((char *)this + 0x0C) = 0;
    *(int *)((char *)this + 0x10) = one;
    *(short *)((char *)this + 0x28) = 0;
    *(short *)((char *)this + 0x2A) = 0;
    __asm__ volatile("" ::: "memory");

    char *desired = (char *)this + 0x2C;
    *(unsigned char *)((char *)this + 0x14) = 0;
    gcDesiredObject_ctor(desired, (cBase *)this);

    *(void **)((char *)this + 0x30) = D_00000338;

    gcDesiredEntityHelper_ctor((char *)this + 0x38, 0, 0, 0);

    *(void **)((char *)this + 0x30) = (void *)0x388A48;
    *(void **)((char *)this + 0x40) = desired;
    *(void **)((char *)this + 0x44) = (void *)0x388568;
    *(unsigned char *)((char *)this + 0x48) = one;
    *(unsigned char *)((char *)this + 0x49) = 0;
    *(int *)((char *)this + 0x4C) = 0;
    int desiredEncoded = (int)desired | 1;
    *(int *)((char *)this + 0x50) = 0;

    int encoded = (int)this | 1;
    *(int *)((char *)this + 0x54) = desiredEncoded;
    *(int *)((char *)this + 0x58) = encoded;
    *(int *)((char *)this + 0x5C) = encoded;
    *(int *)((char *)this + 0x60) = encoded;
    *(int *)((char *)this + 0x64) = encoded;
    *(int *)((char *)this + 0x68) = encoded;
    *(int *)((char *)this + 0x6C) = encoded;
    *(int *)((char *)this + 0x70) = encoded;
    *(int *)((char *)this + 0x74) = encoded;
    *(int *)((char *)this + 0x78) = encoded;
}

void gcDoEntityApplyRigidBodyImpulse::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!D_0009F5E8) {
            if (!D_000385D4) {
                if (!D_000385D8) {
                    if (!D_000385DC) {
                        D_000385DC = cType::InitializeType(
                            (const char *)0x36D894, (const char *)0x36D89C, 1,
                            0, 0, 0, 0, 0);
                    }
                    D_000385D8 = cType::InitializeType(
                        0, 0, 0x6A, D_000385DC, 0, 0, 0, 0);
                }
                D_000385D4 = cType::InitializeType(
                    0, 0, 0x6B, D_000385D8, 0, 0, 0, 0);
            }
            D_0009F5E8 = cType::InitializeType(
                0, 0, 0x1C2, D_000385D4,
                gcDoEntityApplyRigidBodyImpulse::New, 0, 0, 0);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = D_0009F5E8;
        VTableSlot *slot = (VTableSlot *)((char *)vt + 8);
        short voff = slot->offset;
        const cType *(*getType)(void *) = slot->getType;
        const cType *type = getType((char *)other + voff);
        int ok;

        if (myType == 0) {
            ok = 0;
            goto done;
        }
        if (type != 0) {
        loop:
            if (type == myType) {
                ok = 1;
                goto done;
            }
            type = (const cType *)((cTypeNode *)type)->parent;
            if (type != 0) {
                goto loop;
            }
        }
        ok = 0;
    done:
        if (ok != 0) {
            copy = other;
        }
    }
    *this = *(const gcDoEntityApplyRigidBodyImpulse *)copy;
}

void gcDoEntityApplyRigidBodyImpulse::Write(cFile &file) const {
    cWriteBlock wb(file, 5);
    gcAction::Write(file);
    wb.Write(*(int *)((const char *)this + 0x0C));
    wb.Write(*(int *)((const char *)this + 0x10));
    ((const cName *)((const char *)this + 0x14))->Write(wb);

    char *typeInfo = *(char **)((const char *)this + 0x30);
    WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
    short off = rec->offset;
    void *base = (char *)this + 0x2C;
    rec->fn((char *)base + off, wb._file);

    ((const gcDesiredValue *)((const char *)this + 0x58))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x5C))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x60))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x64))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x68))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x6C))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x70))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x74))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x78))->Write(wb);
    wb.End();
}
