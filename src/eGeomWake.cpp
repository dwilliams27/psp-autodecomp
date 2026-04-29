// eGeomWake methods. Inherits eDynamicGeom : eGeom : cObject. In sched=1 zone.
#pragma control sched=1

inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

void cFile_SetCurrentPos(void *, unsigned int);

class eGeom {
public:
    eGeom(cBase *);
    char _eGeomPad[0x90];
};

class eDynamicGeom : public eGeom {
public:
    eDynamicGeom(cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    char _dynPad[0x58];
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class eGeomWake : public eDynamicGeom {
public:
    eGeomWake(cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    char _wakePad[0x30];
};

extern char eGeomWakevirtualtable[];
extern cType *D_000385DC;
extern cType *D_00040FF4;
extern cType *D_000469C0;
extern cType *D_00046C34;

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

// ── eGeomWake::eGeomWake(cBase *) @ 0x00079ff8 ──
eGeomWake::eGeomWake(cBase *base) : eDynamicGeom(base) {
    *(void **)((char *)this + 4) = eGeomWakevirtualtable;
    *(int *)((char *)this + 0xF0) = -1;
    *(int *)((char *)this + 0xF4) = 0;
    *(float *)((char *)this + 0xF8) = 0.0f;
    __asm__ volatile(
        "mfc1 $a0, $f12\n"
        "mfc1 $a1, $f12\n"
        "mfc1 $a2, $f12\n"
        "mtv $a0, S120\n"
        "mtv $a1, S121\n"
        "mtv $a2, S122\n"
        "sv.q C120, 0x100($s0)"
        ::: "a0", "a1", "a2", "memory");
    *(unsigned char *)((char *)this + 0x110) = 1;
}

// ── eGeomWake::Write(cFile &) const @ 0x00079ef0 ──
void eGeomWake::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    this->eDynamicGeom::Write(file);
    wb.End();
}

// ── eGeomWake::Read(cFile &, cMemPool *) @ 0x00079f3c ──
int eGeomWake::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 && this->eDynamicGeom::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── eGeomWake::New(cMemPool *, cBase *) static @ 0x00213134 ──
cBase *eGeomWake::New(cMemPool *pool, cBase *parent) {
    eGeomWake *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eGeomWake *obj = (eGeomWake *)rec->fn(base, 0x120, 0x10, 0, 0);
    if (obj != 0) {
        new (obj) eGeomWake(parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── eGeomWake::GetType(void) const @ 0x002131b0 ──
const cType *eGeomWake::GetType(void) const {
    __asm__ volatile("" ::: "memory");
    if (D_00046C34 == 0) {
        if (D_000469C0 == 0) {
            if (D_00040FF4 == 0) {
                if (D_000385DC == 0) {
                    const char *name = (const char *)0x36CD74;
                    const char *desc = (const char *)0x36CD7C;
                    __asm__ volatile("" : "+r"(name), "+r"(desc));
                    D_000385DC = cType::InitializeType(name, desc, 1,
                                                       0, 0, 0, 0, 0);
                }
                D_00040FF4 = cType::InitializeType(0, 0, 0x16, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_000469C0 = cType::InitializeType(0, 0, 0x17, D_00040FF4,
                                               0, 0, 0, 0);
        }
        const cType *parentType = D_000469C0;
        __asm__ volatile("" : "+r"(parentType));
        __asm__ volatile("" ::: "memory");
        cBase *(*factory)(cMemPool *, cBase *) = eGeomWake::New;
        __asm__ volatile("" : "+r"(factory));
        D_00046C34 = cType::InitializeType(0, 0, 0x257, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046C34;
}
