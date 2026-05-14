inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cFile;
class cMemPool;
class cType;

class cName {
public:
    void Set(const char *, ...);
};

class cFile {
public:
    void SetCurrentPos(unsigned int);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cReadBlock {
public:
    void ReadBase(cMemPool *, cBase *, cBase *&);
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
    int Read(cFile &, cMemPool *);
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

struct HandleEntry {
    char _pad[0x30];
    int handle;
};

struct ResetDispatchRec {
    short offset;
    short pad;
    void (*fn)(void *);
};

class gcEntityCustomAttack : public cObject {
public:
    char _pad[0x44];
    cBase *mField44;

    gcEntityCustomAttack(cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    int Read(cFile &, cMemPool *);
    void Reset(cMemPool *, bool);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int,
                                              bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_0009F434;
extern HandleEntry *D_00038890[];

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

int gcEntityCustomAttack::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19") = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 1, true);

    if (rb[3] != 1 || cObject::Read(file, pool) == 0) {
        ((cFile *)rb[0])->SetCurrentPos(rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    int sp14;
    int value = *(int *)((char *)this + 0x44);
    int tag = value & 1;
    int flag = 0;
    if (tag != 0) {
        flag = 1;
    }

    int outValue;
    if (flag != 0) {
        outValue = 0;
        goto out_done;
    }
    outValue = value;
out_done:
    sp14 = outValue;

    int flag2 = 0;
    if (tag != 0) {
        flag2 = 1;
    }

    int base;
    if (flag2 != 0) {
        base = value & ~1;
    } else {
        base = *(int *)value;
    }

    cMemPool *childPool = cMemPool::GetPoolFromPtr((char *)this + 0x44);
    ((cReadBlock *)rb)->ReadBase(childPool, (cBase *)base, *(cBase **)&sp14);

    int newValue;
    if (sp14 == 0) {
        newValue = base | 1;
    } else {
        newValue = sp14;
    }
    *(int *)((char *)this + 0x44) = newValue;
    __0oKcReadBlockdtv(rb, 2);
    return result;
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

void gcEntityCustomAttack::Reset(cMemPool *, bool) {
    if (((*(short *)((char *)this + 0x1C) == 0) & 0xFF) != 0) {
        int temp_a0 = *(int *)((char *)this + 0x44);
        int var_a2 = 0;
        if (temp_a0 & 1) {
            var_a2 = 1;
        }
        if (var_a2 == 0) {
            goto valid_from_value;
        }
        var_a2 = 0;
        goto valid_done;
    valid_from_value:
        var_a2 = temp_a0 != 0;
        var_a2 = (var_a2 & 0xFF) != 0;
    valid_done:
        if (var_a2 != 0) {
            var_a2 = 0;
            int temp_a1 = temp_a0 & 1;
            if (temp_a1 != 0) {
                var_a2 = 1;
            }
            char *base;
            if (var_a2 == 0) {
                goto slot_from_value;
            }
            base = 0;
            __asm__ volatile("" : "+r"(base));
            goto slot_done;
        slot_from_value:
            base = (char *)temp_a0;
        slot_done:
            int *slot = (int *)(base + 0x10);
            int handle = *slot;
            HandleEntry *found;
            if (handle == 0) {
                found = 0;
            } else {
                int var_a3 = (int)D_00038890[handle & 0xFFFF];
                int var_t0 = 0;
                if (var_a3 != 0) {
                    if (*(int *)(var_a3 + 0x30) == handle) {
                        var_t0 = var_a3;
                    }
                }
                found = (HandleEntry *)var_t0;
            }
            if (found != 0) {
                int var_a1 = 0;
                temp_a1 = temp_a0 & 1;
                if (temp_a1 != 0) {
                    var_a1 = 1;
                }
                char *base2;
                if (var_a1 == 0) {
                    goto slot2_from_value;
                }
                base2 = 0;
                __asm__ volatile("" : "+r"(base2));
                goto slot2_done;
            slot2_from_value:
                base2 = (char *)temp_a0;
            slot2_done:
                int *slot2 = (int *)(base2 + 0x10);
                int handle2 = *slot2;
                HandleEntry *entry2;
                if (handle2 == 0) {
                    entry2 = 0;
                } else {
                    HandleEntry *candidate = D_00038890[handle2 & 0xFFFF];
                    entry2 = 0;
                    if (candidate != 0) {
                        if (candidate->handle == handle2) {
                            entry2 = candidate;
                        }
                    }
                }

                const char *name;
                if (entry2 != 0) {
                    int temp_a0_2 =
                        ((*(short *)((char *)entry2 + 0x1C) == 0) & 0xFF);
                    entry2 = (HandleEntry *)((char *)entry2 + 8);
                    if (temp_a0_2 != 0) {
                        entry2 = (HandleEntry *)0x36DAB8;
                    } else {
                        __asm__ volatile("" ::: "memory");
                    }
                    name = (const char *)entry2;
                } else if (handle2 != 0) {
                    name = (const char *)0x36DAC4;
                } else {
                    name = (const char *)0x36DACC;
                }
                ((cName *)((char *)this + 8))->Set(name);

                ResetDispatchRec *rec =
                    (ResetDispatchRec *)(*(char **)((char *)this + 4) + 0x70);
                short offset = rec->offset;
                void (*fn)(void *) = rec->fn;
                fn((char *)this + offset);
            }
        }
    }
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
