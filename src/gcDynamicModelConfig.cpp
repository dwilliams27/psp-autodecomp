// src/gcDynamicModelConfig.cpp
//
// Three methods of gcDynamicModelConfig (gcAll_psp.obj). The constructor lives
// in src/eVolume.cpp (already matched) so it is not duplicated here.
//
//   - Write(cFile &) const                @ 0x00152344
//   - Read(cFile &, cMemPool *)           @ 0x00152390
//   - New(cMemPool *, cBase *) static     @ 0x00319d3c
//
// Patterns:
//   - Write : cFactory::Write — wrap base-class write in cWriteBlock(file, 1)/End().
//   - Read  : eWeatherEffect::Read — open a cReadBlock(file, 1, true), check the
//             block tag (rb._data[3] == 1), call base Read; on failure rewind file
//             and return 0; on success the dtor runs and we return 1.
//   - New   : eConfigBase::New / gcGeomCurveController::New — pool[9] -> block;
//             block[0x1C] -> allocTable; entry at allocTable+0x28; alloc with
//             size=0x60, alignment=0x10.

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

class gcEntityGeomConfig {
public:
    int base;

    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcDynamicModelConfig : public gcEntityGeomConfig {
public:
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

extern "C" {
    void gcDynamicModelConfig__gcDynamicModelConfig_cBaseptr(void *self, cBase *parent);
}

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

// ── gcDynamicModelConfig::Write(cFile &) const @ 0x00152344 ──
void gcDynamicModelConfig::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    this->gcEntityGeomConfig::Write(file);
    wb.End();
}

// ── gcDynamicModelConfig::Read(cFile &, cMemPool *) @ 0x00152390 ──
int gcDynamicModelConfig::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->gcEntityGeomConfig::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── gcDynamicModelConfig::New(cMemPool *, cBase *) static @ 0x00319d3c ──
cBase *gcDynamicModelConfig::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcDynamicModelConfig *result = 0;
    gcDynamicModelConfig *obj = (gcDynamicModelConfig *)rec->fn(base, 0x60, 0x10, 0, 0);
    if (obj != 0) {
        gcDynamicModelConfig__gcDynamicModelConfig_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
