class cFile;
class cBase;
class cMemPool;
class cType;
class eVolume;

inline void *operator new(unsigned int, void *p) { return p; }

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRec {
    short offset;
    short _pad;
    void (*fn)(void *, int, void *, short);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End();
};

class cObject {
public:
    cObject(cBase *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class eRoom : public cObject {
public:
    static cBase *New(cMemPool *, cBase *);
    eRoom(cBase *);
    const cType *GetType(void) const;
    void Free();
    void RemoveVolume(eVolume *);
    void ClearRoomVolumeList(eVolume *);
};

class eVolumeBody {
public:
    char _pad[0x24];
    int mField24;
};

class gcUI {
public:
    void Write(cFile &) const;
};

class gcViewport {
public:
    void Write(cFile &) const;
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_000468D4;

cBase *eRoom::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eRoom *result = 0;
    eRoom *obj = (eRoom *)rec->fn(base, 0x120, 0x10, 0, 0);
    if (obj != 0) {
        new (obj) eRoom(parent);
        result = obj;
    }
    return (cBase *)result;
}

eRoom::eRoom(cBase *parent) : cObject(parent) {
    *(void **)((char *)this + 4) = (void *)0x380968;
    *(int *)((char *)this + 0x90) = 0;
    *(int *)((char *)this + 0xC0) = 0;
    *(int *)((char *)this + 0xC4) = 0;
    *(int *)((char *)this + 0xC8) = 0;
    *(int *)((char *)this + 0xCC) = 0;
    *(int *)((char *)this + 0xD0) = 0;
    *(int *)((char *)this + 0xD4) = 0;
    *(int *)((char *)this + 0xD8) = 0;
    *(eRoom **)((char *)this + 0xDC) = this;
    *(int *)((char *)this + 0xE0) = 0;
    *(int *)((char *)this + 0xE4) = 0;
    *(int *)((char *)this + 0xE8) = 0;
    *(int *)((char *)this + 0xEC) = 0;
    *(int *)((char *)this + 0xF0) = 0;
    *(unsigned char *)((char *)this + 0xF4) = 0;
    *(int *)((char *)this + 0xF8) = 0;
    *(int *)((char *)this + 0xFC) = 0;
    *(int *)((char *)this + 0x100) = 0;
    *(int *)((char *)this + 0x104) = 0;
    *(int *)((char *)this + 0x108) = 0;
    *(int *)((char *)this + 0x10C) = 0;
    __asm__ volatile(
        "lui $a0, 0x7f7f\n"
        "sw $zero, 0x110(%0)\n"
        "ori $a0, $a0, 0xffff\n"
        "sw $zero, 0x114(%0)\n"
        "mtc1 $a0, $f12\n"
        "sw $zero, 0x118(%0)\n"
        "mfc1 $a0, $f12\n"
        "mfc1 $a1, $f12\n"
        "mfc1 $a2, $f12\n"
        "mtv $a0, S120\n"
        "mtv $a1, S121\n"
        "mtv $a2, S122\n"
        "lui $a0, 0xff7f\n"
        "ori $a0, $a0, 0xffff\n"
        "mtc1 $a0, $f13\n"
        "sv.q C120, 0xA0(%0)\n"
        "mfc1 $a0, $f13\n"
        "mfc1 $a1, $f13\n"
        "mfc1 $a2, $f13\n"
        "mtv $a0, S120\n"
        "mtv $a1, S121\n"
        "mtv $a2, S122\n"
        "sv.q C120, 0xB0(%0)\n"
        :
        : "r"(this)
        : "a0", "a1", "a2", "memory");
}

const cType *eRoom::GetType(void) const {
    if (D_000468D4 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                       (const char *)0x36CD7C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                                   &cNamed::New, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                               0, 0, 0, 0);
        }
        D_000468D4 = cType::InitializeType(0, 0, 0x21C, D_000385E4,
                                           &eRoom::New,
                                           (const char *)0x36CE04,
                                           (const char *)0x36CE0C, 3);
    }
    return D_000468D4;
}

void eRoom::Free() {
    void *obj = *(void **)((char *)this + 0x118);
    if (obj != 0) {
        register DeleteRec *rec __asm__("a2") =
            (DeleteRec *)((char *)(*(void **)((char *)obj + 4)) + 0x50);
        short off = rec->offset;
        void (*fn)(void *, int, void *, short) = rec->fn;
        fn((char *)obj + off, 3, (void *)fn, off);
        *(void **)((char *)this + 0x118) = 0;
    }
    obj = *(void **)((char *)this + 0x104);
    if (obj != 0) {
        register DeleteRec *rec __asm__("a2") =
            (DeleteRec *)((char *)(*(void **)((char *)obj + 4)) + 0x50);
        short off = rec->offset;
        void (*fn)(void *, int, void *, short) = rec->fn;
        fn((char *)obj + off, 3, (void *)fn, off);
        *(void **)((char *)this + 0x104) = 0;
    }
    obj = *(void **)((char *)this + 0xC0);
    if (obj != 0) {
        register DeleteRec *rec __asm__("a2") =
            (DeleteRec *)((char *)(*(void **)((char *)obj + 4)) + 0x50);
        short off = rec->offset;
        void (*fn)(void *, int, void *, short) = rec->fn;
        fn((char *)obj + off, 3, (void *)fn, off);
        *(void **)((char *)this + 0xC0) = 0;
    }
}

void eRoom::RemoveVolume(eVolume *volume) {
    ClearRoomVolumeList(volume);
    ((eVolumeBody *)volume)->mField24 = 0;
}

void gcUI::Write(cFile &f) const {
    cWriteBlock wb(f, 1);
    wb.End();
}

void gcViewport::Write(cFile &f) const {
    cWriteBlock wb(f, 1);
    wb.End();
}

extern "C" {

extern void *_impure_ptr;
void *_fopen_r(void *, const char *, const char *);
double _strtod_r(void *, const char *, char **);

void *fopen(const char *file, const char *mode) {
    return _fopen_r(_impure_ptr, file, mode);
}

double strtod(const char *s, char **endptr) {
    return _strtod_r(_impure_ptr, s, endptr);
}

}
