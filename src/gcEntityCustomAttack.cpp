inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cFile;
class cMemPool;
class cType;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

template <class T> T *dcast(const cBase *);

class cObject {
public:
    cObject(cBase *);
    cObject &operator=(const cObject &);
    void Write(cFile &) const;
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void WriteBase(const cBase *);
    void End(void);
};

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRec {
    short offset;
    short pad;
    void (*fn)(void *, int);
};

struct CopyRec {
    short offset;
    short pad;
    cBase *(*fn)(void *, cMemPool *, cBase *);
};

class gcEntityCustomAttack : public cObject {
public:
    char _pad[0x44];
    cBase *mField44;

    gcEntityCustomAttack(cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_0009F434;

void gcEntityCustomAttack::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cObject::Write(file);
    int val = (int)mField44;
    int flag = 0;
    if (val & 1) {
        flag = 1;
    }
    cBase *ptr;
    if (flag != 0) {
        ptr = 0;
    } else {
        ptr = (cBase *)val;
    }
    wb.WriteBase(ptr);
    wb.End();
}

cBase *gcEntityCustomAttack::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcEntityCustomAttack *result = 0;
    gcEntityCustomAttack *obj =
        (gcEntityCustomAttack *)rec->fn(base, 0x48, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcEntityCustomAttack(parent);
        result = obj;
    }
    return (cBase *)result;
}

void gcEntityCustomAttack::AssignCopy(const cBase *base) {
    gcEntityCustomAttack *temp_v0;
    void *temp_s1;
    int temp_a2;
    int temp_a0;
    int var_a1;
    int var_a1_2;
    int var_a2;
    int temp_s2;
    int var_a0;
    void *temp_a1;
    short temp_a1_2;
    int temp_a0_2;
    cMemPool *temp_a1_3;
    int var_a2_2;
    int var_a0_2;

    temp_v0 = dcast<gcEntityCustomAttack>(base);
    cObject::operator=(*temp_v0);
    temp_s1 = (char *)this + 0x44;
    if ((char *)temp_v0 + 0x44 != temp_s1) {
        temp_a2 = *(int *)temp_s1;
        var_a1 = 1;
        temp_a0 = temp_a2 & 1;
        if (temp_a0 != 0) {
            var_a1 = 0;
        }
        if (var_a1 != 0) {
            var_a1_2 = 0;
            if (temp_a0 != 0) {
                var_a1_2 = 1;
            }
            if (var_a1_2 != 0) {
                var_a2 = temp_a2 & ~1;
                var_a2 |= 1;
            } else {
                var_a2 = *(int *)temp_a2;
                var_a2 |= 1;
            }
            *(int *)temp_s1 = var_a2;
            if (temp_a2 != 0) {
                DeleteRec *entry =
                    (DeleteRec *)(*(char **)(temp_a2 + 4) + 0x50);
                entry->fn((char *)temp_a2 + entry->offset, 3);
            }
        }

        temp_s2 = *(int *)((char *)temp_v0 + 0x44);
        var_a0 = 1;
        if (temp_s2 & 1) {
            var_a0 = 0;
        }
        if (var_a0 != 0) {
            temp_a1 = *(void **)(temp_s2 + 4);
            temp_a1_2 = *(short *)((char *)temp_a1 + 0x10);
            temp_a0_2 = *(int *)temp_s1;
            temp_a1_3 = cMemPool::GetPoolFromPtr(temp_s1);
            var_a2_2 = 0;
            if (temp_a0_2 & 1) {
                var_a2_2 = 1;
            }
            if (var_a2_2 != 0) {
                var_a0_2 = temp_a0_2 & ~1;
            } else {
                var_a0_2 = *(int *)temp_a0_2;
            }
            *(int *)temp_s1 =
                (int)((CopyRec *)((char *)temp_a1 + 0x10))
                    ->fn((char *)temp_s2 + temp_a1_2, temp_a1_3,
                         (cBase *)var_a0_2);
        }
    }
}

const cType *gcEntityCustomAttack::GetType(void) const {
    if (D_0009F434 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                                   &cNamed::New, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                               0, 0, 0, 0);
        }
        D_0009F434 = cType::InitializeType(0, 0, 0x1CD, D_000385E4,
                                           &gcEntityCustomAttack::New,
                                           (const char *)0x36D9A4,
                                           (const char *)0x36D9B4, 5);
    }
    return D_0009F434;
}
