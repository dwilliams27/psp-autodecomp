// src/ePhysicsControllerConfig.cpp
//
// ePhysicsControllerConfig::GetType(void) const @ 0x001f1000  (152B)
//
// Two-level lazy singleton built on cType::InitializeType.  Inner type is the
// shared cBase entry at D_000385DC (already populated by sibling GetType
// callers such as eShape::GetType / ePhysicsController::GetType).  Outer type
// lives at D_000469E8 with type-id 0x23D.
//
// Mirrors the matched eShape::GetType pattern (same address neighbourhood and
// same name/desc strings 0x36CD74 / 0x36CD7C).

class cBase;
class cFile;
class cMemPool;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

extern cType *D_000385DC;
extern cType *D_000469E8;

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cName {
public:
    void Read(cReadBlock &);
};

void cFile_SetCurrentPos(void *, unsigned int);

class ePhysicsControllerConfig {
public:
    const cType *GetType(void) const;
    int Read(cFile &, cMemPool *);
};

#pragma control sched=1
const cType *ePhysicsControllerConfig::GetType(void) const {
    if (D_000469E8 == 0) {
        if (D_000385DC == 0) {
            const char *name = (const char *)0x36CD74;
            const char *desc = (const char *)0x36CD7C;
            __asm__ volatile("" : "+r"(name), "+r"(desc));
            D_000385DC = cType::InitializeType(name, desc, 1, 0, 0, 0, 0, 0);
        }
        D_000469E8 = cType::InitializeType(0, 0, 0x23D, D_000385DC, 0, 0, 0, 0);
    }
    return D_000469E8;
}
#pragma control sched=2

#pragma control sched=1
int ePhysicsControllerConfig::Read(cFile &file, cMemPool *) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] != 1) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return 0;
    }
    ((cName *)((char *)this + 8))->Read(rb);
    return result;
}
#pragma control sched=2
