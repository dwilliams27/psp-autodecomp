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
