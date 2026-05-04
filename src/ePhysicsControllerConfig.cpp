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

class ePhysicsControllerConfig {
public:
    const cType *GetType(void) const;
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
