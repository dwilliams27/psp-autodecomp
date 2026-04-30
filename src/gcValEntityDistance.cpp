class cBase;
class cFile;
class cMemPool;

inline void *operator new(unsigned int, void *p) { return p; }

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class cWriteBlock {
public:
    cFile *file;
    unsigned int _pos;
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(unsigned int);
    void End(void);
};

struct cTypeMethod {
    short offset;
    short pad;
    void *fn;
};

class cType {
public:
    char pad[0x1C];
    cType *mParent;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cName {
public:
    void Write(cWriteBlock &) const;
};

class gcDesiredObject {
public:
    int _parent;
    cType *mType;

    gcDesiredObject(cBase *);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcDesiredEntityHelper {
public:
    gcDesiredEntityHelper(int, int, int);
};

class gcValue {
public:
    void Write(cFile &) const;
};

class gcValEntityDistance : public gcValue {
public:
    gcValEntityDistance(cBase *);
    gcValEntityDistance &operator=(const gcValEntityDistance &);
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    const cType *GetType(void) const;
};

struct DispatchEntry {
    short offset;
    short pad;
    cType *(*fn)(void *);
};

extern "C" void gcDesiredObject_ctor(void *, void *)
    __asm__("__0oPgcDesiredObjectctP6FcBase");
extern "C" void gcDesiredEntityHelper_ctor(void *, int, int, int)
    __asm__("gcDesiredEntityHelper__gcDesiredEntityHelper_gcDesiredEntityHelper__gcPrimary_gcDesiredEntityHelper__gcRelationship_gcDesiredEntityHelper__gcRelationship__0011B714");
extern char D_00007028[];

gcValEntityDistance::gcValEntityDistance(cBase *parent) {
    ((cBase **)this)[0] = parent;
    ((void **)this)[1] = D_00007028;

    char *first = (char *)this + 8;
    gcDesiredObject_ctor(first, this);
    register void *type1 asm("a0");
    __asm__ volatile("lui %0,0x0\n\taddiu %0,%0,0x338" : "=r"(type1));
    ((void **)this)[3] = type1;
    gcDesiredEntityHelper_ctor((char *)this + 0x14, 1, 0, 0);
    register void *helper_desc asm("a0");
    __asm__ volatile("lui %0,0x39\n\taddiu %0,%0,-0x75b8"
                     : "=r"(helper_desc));
    ((void **)this)[3] = helper_desc;
    register void *desired_desc asm("a0");
    __asm__ volatile("lui %0,0x39" : "=r"(desired_desc));
    ((char **)this)[7] = first;
    __asm__ volatile("addiu %0,%0,-0x7a98" : "+r"(desired_desc));
    ((void **)this)[8] = desired_desc;
    ((char *)this)[0x24] = 1;
    ((char *)this)[0x25] = 0;
    ((int *)this)[10] = 0;
    int first_bits = (int)first | 1;
    ((int *)this)[11] = 0;
    ((int *)this)[12] = first_bits;

    char *second = (char *)this + 0x34;
    gcDesiredObject_ctor(second, this);
    ((void **)this)[14] = type1;
    gcDesiredEntityHelper_ctor((char *)this + 0x40, 1, 0, 0);
    ((void **)this)[14] = helper_desc;
    ((char **)this)[18] = second;
    ((void **)this)[19] = desired_desc;
    ((char *)this)[0x50] = 1;
    ((char *)this)[0x51] = 0;
    ((int *)this)[21] = 0;
    int second_bits = (int)second | 1;
    ((int *)this)[22] = 0;
    ((int *)this)[23] = second_bits;

    gcDesiredObject_ctor((char *)this + 0x60, this);
    ((int *)this)[27] = 7;
    ((int *)this)[28] = 0;
    ((void **)this)[25] = (void *)0x389760;
    *(short *)((char *)this + 0x88) = 0;
    *(short *)((char *)this + 0x8A) = 0;
    ((char *)this)[0x74] = 0;
    *(short *)((char *)this + 0xA0) = 0;
    *(short *)((char *)this + 0xA2) = 0;
    ((char *)this)[0x8C] = 0;
    int three = 3;
    ((int *)this)[41] = 0;
    ((int *)this)[42] = three;
    ((char *)this)[0xAC] = 0;
    int self_bits = (int)this | 1;
    ((char *)this)[0xAD] = 0;
    ((int *)this)[44] = self_bits;
    ((int *)this)[45] = self_bits;
}

cBase *gcValEntityDistance::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcValEntityDistance *result = 0;
    gcValEntityDistance *obj =
        (gcValEntityDistance *)rec->fn(base, 0xB8, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcValEntityDistance(parent);
        result = obj;
    }
    return (cBase *)result;
}

// 0x0032c31c
static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValEntityDistance;

const cType *gcValEntityDistance::GetType(void) const {
    if (!type_gcValEntityDistance) {
        if (!type_value) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType((const char *)0x36D894,
                                                      (const char *)0x36D89C,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base,
                                                        0, 0, 0, 0);
            }
            type_value = cType::InitializeType(0, 0, 0x6C, type_expression,
                                               0, 0, 0, 0x80);
        }
        type_gcValEntityDistance = cType::InitializeType(
            0, 0, 0xE5, type_value, gcValEntityDistance::New, 0, 0, 0);
    }
    return type_gcValEntityDistance;
}

void gcValEntityDistance::AssignCopy(const cBase *base) {
    const gcValEntityDistance *other = 0;

    if (base != 0) {
        if (!type_gcValEntityDistance) {
            if (!type_value) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            (const char *)0x36D894, (const char *)0x36D89C,
                            1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_value = cType::InitializeType(
                    0, 0, 0x6C, type_expression, 0, 0, 0, 0x80);
            }
            type_gcValEntityDistance = cType::InitializeType(
                0, 0, 0xE5, type_value, gcValEntityDistance::New, 0, 0, 0);
        }

        DispatchEntry *entry =
            (DispatchEntry *)((char *)*(void **)((char *)base + 4) + 8);
        cType *wanted = type_gcValEntityDistance;
        cType *type = entry->fn((char *)base + entry->offset);
        int ok;

        if (wanted == 0) {
            ok = 0;
        } else if (type != 0) {
        loop:
            if (type == wanted) {
                ok = 1;
            } else {
                type = type->mParent;
                if (type != 0) {
                    goto loop;
                }
                goto fail;
            }
        } else {
        fail:
            ok = 0;
        }
        if (ok != 0) {
            other = (const gcValEntityDistance *)base;
        }
    }

    operator=(*other);
}

// 0x0032c434
void gcValEntityDistance::Write(cFile &file) const {
    cWriteBlock wb(file, 7);
    gcValue::Write(file);

    const cTypeMethod *write0 =
        (const cTypeMethod *)((const char *)((const gcDesiredObject *)((const char *)this + 8))->mType + 0x28);
    char *base0 = (char *)this + 8;
    typedef void (*WriteFn)(void *, cFile *);
    ((WriteFn)write0->fn)(base0 + write0->offset, wb.file);

    const cTypeMethod *write1 =
        (const cTypeMethod *)((const char *)((const gcDesiredObject *)((const char *)this + 0x34))->mType + 0x28);
    char *base1 = (char *)this + 0x34;
    ((WriteFn)write1->fn)(base1 + write1->offset, wb.file);

    const cTypeMethod *write2 =
        (const cTypeMethod *)((const char *)((const gcDesiredObject *)((const char *)this + 0x60))->mType + 0x28);
    char *base2 = (char *)this + 0x60;
    ((WriteFn)write2->fn)(base2 + write2->offset, wb.file);

    wb.Write(*(const unsigned int *)((const char *)this + 0xA4));
    ((const cName *)((const char *)this + 0x74))->Write(wb);
    ((const cName *)((const char *)this + 0x8C))->Write(wb);
    wb.Write(*(const bool *)((const char *)this + 0xAC));
    wb.Write(*(const unsigned int *)((const char *)this + 0xA8));
    wb.Write(*(const bool *)((const char *)this + 0xAD));
    ((const gcDesiredValue *)((const char *)this + 0xB0))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0xB4))->Write(wb);
    wb.End();
}
