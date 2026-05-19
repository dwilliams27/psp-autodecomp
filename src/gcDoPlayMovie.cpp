#include "cBase.h"

class cFile;
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

class cWriteBlock {
public:
    int _data[2];

    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(int);
    void WriteBase(const cBase *);
    void End(void);
};

class cName {
public:
    void Write(cWriteBlock &) const;
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

class cGUID {
public:
    void Write(cWriteBlock &) const;
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcDoPlayMovie {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    static void operator delete(void *);
    ~gcDoPlayMovie(void);
};

extern const char gcDoPlayMovie_base_name[] asm("D_0036D894");
extern const char gcDoPlayMovie_base_desc[] asm("D_0036D89C");
extern char gcDoPlayMovievirtualtable[];

void gcAction_Write(const gcDoPlayMovie *, cFile &);
extern "C" void gcAction_dtor(void *, int) asm("__0oIgcActiondtv");

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

inline void gcDoPlayMovie::operator delete(void *ptr) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    DtorDeleteRecord *slot = (DtorDeleteRecord *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoPlayMovie asm("D_0009F6D4");

const cType *gcDoPlayMovie::GetType(void) const {
    if (!type_gcDoPlayMovie) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoPlayMovie_base_name, gcDoPlayMovie_base_desc, 1,
                        0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoPlayMovie = cType::InitializeType(
            0, 0, 0x152, type_action, gcDoPlayMovie::New, 0, 0, 0);
    }
    return type_gcDoPlayMovie;
}

void gcDoPlayMovie::Write(cFile &file) const {
    cWriteBlock wb(file, 5);
    gcAction_Write(this, file);
    ((const cName *)((const char *)this + 0x0C))->Write(wb);
    wb.Write(*(const bool *)((const char *)this + 0x28));
    wb.Write(*(const int *)((const char *)this + 0x2C));
    ((const cHandle *)((const char *)this + 0x30))->Write(wb);
    ((const cHandle *)((const char *)this + 0x34))->Write(wb);
    ((const cGUID *)((const char *)this + 0x38))->Write(wb);

    int value0 = *(int *)((const char *)this + 0x40);
    int flag0 = 0;
    int tagged0 = value0 & 1;
    if (tagged0 != 0) {
        flag0 = 1;
    }
    cBase *ptr0;
    if (flag0 != 0) {
        ptr0 = 0;
    } else {
        ptr0 = (cBase *)value0;
    }
    wb.WriteBase(ptr0);

    ((const gcDesiredValue *)((const char *)this + 0x44))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x48))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x4C))->Write(wb);

    int value1 = *(int *)((const char *)this + 0x24);
    int flag1 = 0;
    int tagged1 = value1 & 1;
    if (tagged1 != 0) {
        flag1 = 1;
    }
    cBase *ptr1;
    if (flag1 != 0) {
        ptr1 = 0;
    } else {
        ptr1 = (cBase *)value1;
    }
    wb.WriteBase(ptr1);

    ((const gcDesiredValue *)((const char *)this + 0x50))->Write(wb);
    ((const cHandle *)((const char *)this + 0x54))->Write(wb);
    wb.End();
}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oNgcDoPlayMoviedtv, 0x28c\n");

gcDoPlayMovie::~gcDoPlayMovie(void) {
    *(void **)((char *)this + 4) = gcDoPlayMovievirtualtable;
    char *p50 = (char *)this + 0x50;
    char *p4C = (char *)this + 0x4C;
    char *p48 = (char *)this + 0x48;
    char *p44 = (char *)this + 0x44;
    char *p40 = (char *)this + 0x40;
    char *p24 = (char *)this + 0x24;

    if ((void *)p50 != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x50);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x50) = 0;
            }
        }
    }

    if ((void *)p4C != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x4C);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x4C) = 0;
            }
        }
    }

    if ((void *)p48 != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x48);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x48) = 0;
            }
        }
    }

    if ((void *)p44 != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x44);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x44) = 0;
            }
        }
    }

    if ((void *)p40 != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x40);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x40) = 0;
            }
        }
    }

    if ((void *)p24 != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x24);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x24) = 0;
            }
        }
    }

    gcAction_dtor(this, 0);
}
