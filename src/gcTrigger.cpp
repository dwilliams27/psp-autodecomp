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
    cObject &operator=(const cObject &);
    void Write(cFile &) const;
};

class gcEvent {
public:
    gcEvent &operator=(const gcEvent &);
};

template <class T> class cArrayBase {
public:
    cArrayBase &operator=(const cArrayBase &);
};

class gcEnumeration;
class gcEnumerationEntry;
template <class T> class cSubHandleT;
template <class A, class B> class cHandlePairT;
template <class T> T *dcast(const cBase *);
typedef int v4sf_t __attribute__((mode(V4SF)));

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
    void AssignCopy(const cBase *);
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

void gcTrigger::AssignCopy(const cBase *base) {
    gcTrigger *other = dcast<gcTrigger>(base);

    ((cObject *)this)->operator=(*(const cObject *)other);
    ((gcEvent *)((char *)this + 0x44))->operator=(
        *(const gcEvent *)((char *)other + 0x44));
    ((gcEvent *)((char *)this + 0x60))->operator=(
        *(const gcEvent *)((char *)other + 0x60));
    ((gcEvent *)((char *)this + 0x7C))->operator=(
        *(const gcEvent *)((char *)other + 0x7C));
    ((cArrayBase<cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > > *)((char *)this + 0x98))->operator=(
        *(const cArrayBase<cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > > *)((char *)other + 0x98));

    int *src9C = (int *)((char *)other + 0x9C);
    int value9C = *src9C;
    int *dst9C = (int *)((char *)this + 0x9C);
    *dst9C = value9C;
    *(unsigned char *)((char *)this + 0xA0) =
        *(unsigned char *)((char *)other + 0xA0);
    int valueA4 = *(int *)((char *)other + 0xA4);
    int *srcA8 = (int *)((char *)other + 0xA8);
    *(int *)((char *)this + 0xA4) = valueA4;
    int valueA8 = *srcA8;
    int *dstA8 = (int *)((char *)this + 0xA8);
    *dstA8 = valueA8;
    *(int *)((char *)this + 0xB8) = *(int *)((char *)other + 0xB8);
    *(float *)((char *)this + 0xBC) = *(float *)((char *)other + 0xBC);
    *(v4sf_t *)((char *)this + 0xC0) =
        *(const v4sf_t *)((char *)other + 0xC0);
    *(int *)((char *)this + 0xD0) = *(int *)((char *)other + 0xD0);
    *(int *)((char *)this + 0xD4) = *(int *)((char *)other + 0xD4);
    *(v4sf_t *)((char *)this + 0x110) =
        *(const v4sf_t *)((char *)other + 0x110);
    *(v4sf_t *)((char *)this + 0xE0) =
        *(const v4sf_t *)((char *)other + 0xE0);
    *(v4sf_t *)((char *)this + 0xF0) =
        *(const v4sf_t *)((char *)other + 0xF0);
    *(v4sf_t *)((char *)this + 0x100) =
        *(const v4sf_t *)((char *)other + 0x100);
    *(int *)((char *)this + 0x120) = *(int *)((char *)other + 0x120);
    *(unsigned char *)((char *)this + 0x124) =
        *(unsigned char *)((char *)other + 0x124);
    *(unsigned char *)((char *)this + 0x125) =
        *(unsigned char *)((char *)other + 0x125);
    *(int *)((char *)this + 0x128) = *(int *)((char *)other + 0x128);
    *(int *)((char *)this + 0x12C) = *(int *)((char *)other + 0x12C);
    *(int *)((char *)this + 0x130) = *(int *)((char *)other + 0x130);
    *(int *)((char *)this + 0x134) = *(int *)((char *)other + 0x134);
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
