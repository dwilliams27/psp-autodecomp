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

class gcEntityCustomAnimation : public cObject {
public:
    char _pad[0x44];
    cBase *mField44;

    gcEntityCustomAnimation(cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00099AC0;

void gcEntityCustomAnimation::Write(cFile &file) const {
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

cBase *gcEntityCustomAnimation::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcEntityCustomAnimation *result = 0;
    gcEntityCustomAnimation *obj =
        (gcEntityCustomAnimation *)rec->fn(base, 0x48, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcEntityCustomAnimation(parent);
        result = obj;
    }
    return (cBase *)result;
}

void gcEntityCustomAnimation::AssignCopy(const cBase *base) {
    gcEntityCustomAnimation *temp_v0 = dcast<gcEntityCustomAnimation>(base);
    gcEntityCustomAnimation *temp_s2_ptr = temp_v0;
    void *temp_s1 = (char *)this + 0x44;
    cObject::operator=(*temp_v0);
    if ((char *)temp_s2_ptr + 0x44 != temp_s1) {
        int temp_a2 = (int)mField44;
        int var_a1 = 1;
        int temp_a0 = temp_a2 & 1;
        if (temp_a0 != 0) {
            var_a1 = 0;
        }
        if (var_a1 != 0) {
            int temp_a3 = temp_a0;
            int temp_a0_2 = temp_a2;
            int var_a1_2 = 0;
            int var_a2;
            if (temp_a3 != 0) {
                var_a1_2 = 1;
            }
            if (var_a1_2 != 0) {
                var_a2 = temp_a0_2 & ~1;
            } else {
                var_a2 = *(int *)temp_a0_2;
            }
            mField44 = (cBase *)(var_a2 | 1);
            if (temp_a0_2 != 0) {
                DeleteRec *temp_a2_2 =
                    (DeleteRec *)(*(char **)(temp_a0_2 + 4) + 0x50);
                temp_a2_2->fn((char *)temp_a0_2 + temp_a2_2->offset, 3);
            }
        }

        int temp_s2 = (int)temp_s2_ptr->mField44;
        int var_a0 = 1;
        if (temp_s2 & 1) {
            var_a0 = 0;
        }
        if (var_a0 != 0) {
            void *temp_a1 = *(void **)(temp_s2 + 4);
            temp_a1 = (char *)temp_a1 + 0x10;
            short temp_a1_2 = *(short *)temp_a1;
            temp_s2 += temp_a1_2;
            cMemPool *temp_a1_3 = cMemPool::GetPoolFromPtr(temp_s1);
            int temp_a0_2 = (int)mField44;
            int var_a2_2 = 0;
            int var_a0_2;
            if (temp_a0_2 & 1) {
                var_a2_2 = 1;
            }
            if (var_a2_2 != 0) {
                var_a0_2 = temp_a0_2 & ~1;
            } else {
                var_a0_2 = *(int *)temp_a0_2;
            }
            mField44 = (cBase *)(*(cBase * (**)(void *, cMemPool *, cBase *))
                                     ((char *)temp_a1 + 4))(
                (void *)temp_s2, temp_a1_3, (cBase *)var_a0_2);
        }
    }
}

const cType *gcEntityCustomAnimation::GetType(void) const {
    if (D_00099AC0 == 0) {
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
        D_00099AC0 = cType::InitializeType(0, 0, 0x196, D_000385E4,
                                           &gcEntityCustomAnimation::New,
                                           (const char *)0x36D8D8,
                                           (const char *)0x36D8EC, 5);
    }
    return D_00099AC0;
}
