// gcPartialBodyController — derives from gcPartialEntityController.
// Decompiled functions:
//   0x0014024c gcPartialBodyController::Write(cFile &) const
//   0x00140298 gcPartialBodyController::Read(cFile &, cMemPool *)
//   0x002a5d90 gcPartialBodyController::New(cMemPool *, cBase *) static

class cBase;
class cFile;
class cMemPool;

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

class gcPartialEntityController {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcPartialBodyController : public gcPartialEntityController {
public:
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

extern "C" {
    void gcPartialBodyController__gcPartialBodyController_cBaseptr(void *self, cBase *parent);
}

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

// =====================================================================
// 0x0014024c — gcPartialBodyController::Write(cFile &) const
// =====================================================================
void gcPartialBodyController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    this->gcPartialEntityController::Write(file);
    wb.End();
}

// =====================================================================
// 0x00140298 — gcPartialBodyController::Read(cFile &, cMemPool *)
// =====================================================================
int gcPartialBodyController::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->gcPartialEntityController::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// =====================================================================
// 0x002a5d90 — gcPartialBodyController::New(cMemPool *, cBase *) static
// =====================================================================
cBase *gcPartialBodyController::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcPartialBodyController *result = 0;
    gcPartialBodyController *obj = (gcPartialBodyController *)rec->fn(base, 0x54, 4, 0, 0);
    if (obj != 0) {
        gcPartialBodyController__gcPartialBodyController_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
