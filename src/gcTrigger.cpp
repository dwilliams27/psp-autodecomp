// gcTrigger -- trigger object helpers.

class cBase;
class cFile;
class cMemPool;
class cType;

inline void *operator new(unsigned int, void *p) { return p; }

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

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(int);
    void Write(unsigned int);
    void End(void);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

class cObject {
public:
    void Write(cFile &) const;
};

struct TypeDispatchEntry {
    short offset;
    short pad;
    void (*fn)(void *, int);
};

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_0009F4A4;

class gcTrigger {
public:
    gcTrigger(cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;

    static cBase *New(cMemPool *, cBase *);
};

// -- gcTrigger::New(cMemPool *, cBase *) static @ 0x00276f70 --
cBase *gcTrigger::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcTrigger *result = 0;
    gcTrigger *obj = (gcTrigger *)rec->fn(base, 0x140, 0x10, 0, 0);
    if (obj != 0) {
        new (obj) gcTrigger(parent);
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcTrigger::GetType(void) const {
    if (D_0009F4A4 == 0) {
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
        D_0009F4A4 = cType::InitializeType(0, 0, 0x9B, D_000385E4,
                                           &gcTrigger::New,
                                           (const char *)0x36D9EC,
                                           (const char *)0x36D9F8, 0);
    }
    return D_0009F4A4;
}

void gcTrigger::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    ((const cObject *)this)->Write(file);
    wb.Write(*(const unsigned char *)((const char *)this + 0xA0));

    {
        char *typePtr = *(char **)((const char *)this + 0x48);
        TypeDispatchEntry *entry = (TypeDispatchEntry *)(typePtr + 0x28);
        char *base = (char *)this + 0x44;
        entry->fn(base + entry->offset, wb._data[0]);
    }

    {
        char *typePtr = *(char **)((const char *)this + 0x64);
        TypeDispatchEntry *entry = (TypeDispatchEntry *)(typePtr + 0x28);
        char *base = (char *)this + 0x60;
        entry->fn(base + entry->offset, wb._data[0]);
    }

    {
        char *typePtr = *(char **)((const char *)this + 0x80);
        TypeDispatchEntry *entry = (TypeDispatchEntry *)(typePtr + 0x28);
        char *base = (char *)this + 0x7C;
        entry->fn(base + entry->offset, wb._data[0]);
    }

    {
        char *typePtr = *(char **)((const char *)this + 0xB4);
        TypeDispatchEntry *entry = (TypeDispatchEntry *)(typePtr + 0x28);
        char *base = (char *)this + 0xB0;
        entry->fn(base + entry->offset, wb._data[0]);
    }

    wb.Write(*(const int *)((const char *)this + 0xA4));
    ((const cHandle *)((const char *)this + 0xA8))->Write(wb);

    {
        void *temp_s2 = *(void *const *)((const char *)this + 0x98);
        int var_a0 = 0;
        if (temp_s2 != 0) {
            var_a0 = ((int *)temp_s2)[-1] & 0x3FFFFFFF;
        }
        wb.Write(var_a0);
    }

    {
        void *temp_s2_2 = *(void *const *)((const char *)this + 0x98);
        int var_s1 = 0;
        if (temp_s2_2 != 0) {
            var_s1 = ((int *)temp_s2_2)[-1] & 0x3FFFFFFF;
        }

        int var_s2 = 0;
        if (!(var_s2 < var_s1)) goto write_done;
        int var_s3 = (int)temp_s2_2 + var_s2;
write_loop:
        ((const cHandle *)var_s3)->Write(wb);
        wb.Write(*(unsigned int *)(var_s3 + 4));
        var_s2 += 1;
        if (var_s2 < var_s1) {
            var_s3 += 8;
            goto write_loop;
        }
write_done:
        ;
    }

    wb.Write(*(const unsigned int *)((const char *)this + 0x9C));
    wb.End();
}
