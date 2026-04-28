class cFile;
class cBase;
class cMemPool;
class eVolume;

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

class eRoom {
public:
    static cBase *New(cMemPool *, cBase *);
    eRoom(cBase *);
    void Free();
    void RemoveVolume(eVolume *);
    void ClearRoomVolumeList(eVolume *);
};

extern "C" {
    void eRoom__eRoom_cBaseptr__0003D5D4(void *self, cBase *parent);
}

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

cBase *eRoom::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eRoom *result = 0;
    eRoom *obj = (eRoom *)rec->fn(base, 0x120, 0x10, 0, 0);
    if (obj != 0) {
        eRoom__eRoom_cBaseptr__0003D5D4(obj, parent);
        result = obj;
    }
    return (cBase *)result;
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
