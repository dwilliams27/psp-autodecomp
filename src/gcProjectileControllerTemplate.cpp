// gcProjectileControllerTemplate — gcAll_psp.obj
//
// Functions:
//   gcProjectileControllerTemplate::Write(cFile &) const @ 0x00155734 (172B)
//   gcProjectileControllerTemplate::gcProjectileControllerTemplate(cBase *) @ 0x00155B1C (100B)
//   gcProjectileControllerTemplate::New(cMemPool *, cBase *) static @ 0x0031EC28 (124B)

class cBase;
class cFile;
class cFileHandle;
class cMemPool;
class cType;

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

class cReadBlock {
public:
    int _data[5];
};

inline void *operator new(unsigned int, void *p) { return p; }

class cType {
public:
    char _pad[0x1C];
    const cType *mParent;

    static cType *InitializeType(const char *, const char *, unsigned int, const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class cWriteBlock {
public:
    int _data[2];

    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(unsigned int);
    void Write(float);
    void End(void);
};

class cBaseArray {
public:
    int _count;
    cBase *_owner;
    cBaseArray &operator=(const cBaseArray &);
};

struct DispatchEntry {
    short offset;
    short _pad;
    cType *(*fn)(void *, short, void *);
};

class gcEntityControllerTemplate {
public:
    gcEntityControllerTemplate(cBase *);
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    char _pad[0x50];
};

class gcProjectileControllerTemplate : public gcEntityControllerTemplate {
public:
    unsigned int m50;
    float m54;
    float m58;
    float m5C;
    float m60;
    float m64;
    unsigned int m68;
    bool m6C;
    char _pad6D[3];

    gcProjectileControllerTemplate(cBase *);
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_0009A400;
extern cType *D_0009F448;
extern cType *D_0009F7A8;
extern char gcProjectileControllerTemplatevirtualtable[];
extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

// ── gcProjectileControllerTemplate::Write(cFile &) const @ 0x00155734 ──
void gcProjectileControllerTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 0xC);
    this->gcEntityControllerTemplate::Write(file);
    wb.Write(m68);
    wb.Write(m50);
    wb.Write(m54);
    wb.Write(m58);
    wb.Write(m5C);
    wb.Write(m60);
    wb.Write(m64);
    wb.Write(m6C);
    wb.End();
}

#pragma control sched=2
int gcProjectileControllerTemplate::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];

    __0oKcReadBlockctR6FcFileUib(rb, file, 0xC, true);
    if ((unsigned int)rb[3] >= 0xD || (unsigned int)rb[3] < 0xA ||
        this->gcEntityControllerTemplate::Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    register unsigned int *m68p __asm__("$18") =
        (unsigned int *)((char *)this + 0x68);
    cFileSystem::Read(*(cFileHandle **)rb[0], m68p, 4);
    if ((unsigned int)rb[3] < 0xC) {
        register char *parent __asm__("$19") = *(char **)this;
        register char *entity __asm__("$20") = 0;
        if (parent != 0) {
            if (D_0009F448 == 0) {
                if (D_000385E4 == 0) {
                    if (D_000385E0 == 0) {
                        if (D_000385DC == 0) {
                            D_000385DC = cType::InitializeType(
                                (const char *)0x36D894,
                                (const char *)0x36D89C, 1, 0, 0, 0, 0, 0);
                        }
                        D_000385E0 = cType::InitializeType(
                            0, 0, 2, D_000385DC,
                            (cBase *(*)(cMemPool *, cBase *))0x1C3C58,
                            0, 0, 0);
                    }
                    D_000385E4 = cType::InitializeType(
                        0, 0, 3, D_000385E0, 0, 0, 0, 0);
                }
                D_0009F448 = cType::InitializeType(
                    0, 0, 0x8E, D_000385E4,
                    (cBase *(*)(cMemPool *, cBase *))0x26882C,
                    (const char *)0x36D9B8, (const char *)0x36D9C8, 5);
            }

            char *classDesc = (char *)((void **)parent)[1];
            cType *wanted = D_0009F448;
            DispatchEntry *entry = (DispatchEntry *)(classDesc + 8);
            short off = entry->offset;
            cType *(*fn)(void *, short, void *) = entry->fn;
            cType *type = fn(parent + off, off, (void *)fn);
            int ok;

            if (wanted == 0) {
                ok = 0;
            } else if (type != 0) {
            loop:
                if (type == wanted) {
                    ok = 1;
                } else {
                    type = (cType *)type->mParent;
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
                entity = parent;
            }
        }

        *m68p = *(unsigned int *)(entity + 0xCC);
    }

    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x50, 4);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x54, 4);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x58, 4);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x5C, 4);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x60, 4);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x64, 4);

    if ((unsigned int)rb[3] >= 0xB) {
        char sp14;
        cFileSystem::Read(*(cFileHandle **)rb[0], &sp14, 1);
        *(unsigned char *)((char *)this + 0x6C) = sp14 != 0;
    }

    __0oKcReadBlockdtv(rb, 2);
    return result;
}

// ── gcProjectileControllerTemplate::gcProjectileControllerTemplate(cBase *) @ 0x00155B1C ──
gcProjectileControllerTemplate::gcProjectileControllerTemplate(cBase *parent)
    : gcEntityControllerTemplate(parent) {
    *(void **)((char *)this + 4) = gcProjectileControllerTemplatevirtualtable;
    m50 = 0;
    m54 = 0.0f;
    m58 = 1.0f;
    m5C = 0.0f;
    m60 = 0.0f;
    m64 = 0.0f;
    m68 = (unsigned int)-1;
    m6C = false;
}

// ── gcProjectileControllerTemplate::New(cMemPool *, cBase *) static @ 0x0031EC28 ──
cBase *gcProjectileControllerTemplate::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcProjectileControllerTemplate *result = 0;
    gcProjectileControllerTemplate *obj = (gcProjectileControllerTemplate *)rec->fn(base, 0x70, 0x10, 0, 0);
    if (obj != 0) {
        new (obj) gcProjectileControllerTemplate(parent);
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcProjectileControllerTemplate::GetType(void) const {
    if (D_0009F7A8 == 0) {
        if (D_0009A400 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009A400 = cType::InitializeType(0, 0, 0x9A, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F7A8 = cType::InitializeType(0, 0, 0xF1, D_0009A400,
                                           &gcProjectileControllerTemplate::New,
                                           0, 0, 0);
    }
    return D_0009F7A8;
}

typedef int v4sf_t __attribute__((mode(V4SF)));

void gcProjectileControllerTemplate::AssignCopy(const cBase *base) {
    const gcProjectileControllerTemplate *other = 0;

    if (base != 0) {
        if (D_0009F7A8 == 0) {
            if (D_0009A400 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType(
                        (const char *)0x36D894, (const char *)0x36D89C,
                        1, 0, 0, 0, 0, 0);
                }
                D_0009A400 = cType::InitializeType(
                    0, 0, 0x9A, D_000385DC, 0, 0, 0, 0);
            }
            D_0009F7A8 = cType::InitializeType(
                0, 0, 0xF1, D_0009A400, &gcProjectileControllerTemplate::New,
                0, 0, 0);
        }

        void *classDesc = *(void **)((const char *)base + 4);
        cType *target = D_0009F7A8;
        DispatchEntry *entry = (DispatchEntry *)((char *)classDesc + 8);
        short offset = entry->offset;
        cType *(*fn)(void *, short, void *) = entry->fn;
        cType *type = fn((char *)base + offset, offset, fn);
        int isValid;

        if (target != 0) {
            goto have_target;
        }
        isValid = 0;
        goto cast_done;

have_target:
        if (type != 0) {
loop_cast:
            if (type == target) {
                isValid = 1;
            } else {
                type = (cType *)type->mParent;
                if (type != 0) {
                    goto loop_cast;
                }
                goto invalid_cast;
            }
        } else {
invalid_cast:
            isValid = 0;
        }

cast_done:
        if (isValid != 0) {
            other = (const gcProjectileControllerTemplate *)base;
        }
    }

    const cBaseArray &srcArr0 =
        *(const cBaseArray *)((const char *)other + 0x08);
    ((cBaseArray *)((char *)this + 0x08))->operator=(srcArr0);

    int i = 0;
    int *dst = (int *)((char *)this + 0x10);
    const int *src = (const int *)((const char *)other + 0x10);
    do {
        i++;
        *dst = *src;
        dst++;
        src++;
    } while (i < 2);

    *(unsigned int *)((char *)this + 0x18) =
        *(const unsigned int *)((const char *)other + 0x18);

    ((cBaseArray *)((char *)this + 0x1C))->operator=(
        *(const cBaseArray *)((const char *)other + 0x1C));

    *(v4sf_t *)((char *)this + 0x30) =
        *(const v4sf_t *)((const char *)other + 0x30);
    *(float *)((char *)this + 0x40) =
        *(const float *)((const char *)other + 0x40);
    *(float *)((char *)this + 0x44) =
        *(const float *)((const char *)other + 0x44);
    *(float *)((char *)this + 0x48) =
        *(const float *)((const char *)other + 0x48);
    *(unsigned int *)((char *)this + 0x50) =
        *(const unsigned int *)((const char *)other + 0x50);
    *(float *)((char *)this + 0x54) =
        *(const float *)((const char *)other + 0x54);
    *(float *)((char *)this + 0x58) =
        *(const float *)((const char *)other + 0x58);
    const char *src68 = (const char *)other + 0x68;
    char *dst68 = (char *)this + 0x68;
    *(float *)((char *)this + 0x5C) =
        *(const float *)((const char *)other + 0x5C);
    *(float *)((char *)this + 0x60) =
        *(const float *)((const char *)other + 0x60);
    *(float *)((char *)this + 0x64) =
        *(const float *)((const char *)other + 0x64);
    *(unsigned int *)dst68 = *(const unsigned int *)src68;
    *(unsigned char *)((char *)this + 0x6C) =
        *(const unsigned char *)((const char *)other + 0x6C);
}
