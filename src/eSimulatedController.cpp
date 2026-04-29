#include "eSimulatedController.h"
#include "eBipedController.h"
#include "cFile.h"

#pragma control sched=1

class cBase;
class cMemPool;
class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class ePhysicsController {
public:
    void Write(cFile &) const;
};

class eSimulatedControllerConfig {
public:
    const cType *GetType(void) const;
};

extern cType *D_000385DC;
extern cType *D_000469D8;
extern cType *D_000469E8;
extern cType *D_00046BB4;
extern cType *D_00046BD4;
extern cType *D_00046BF8;

void eSimulatedController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const ePhysicsController *)this)->Write(file);
    wb.End();
}

const cType *eBipedController::GetType(void) const {
    if (D_00046BB4 == 0) {
        if (D_000469D8 == 0) {
            if (D_000385DC == 0) {
                const char *name = (const char *)0x36CD74;
                const char *desc = (const char *)0x36CD7C;
                __asm__ volatile("" : "+r"(name), "+r"(desc));
                D_000385DC = cType::InitializeType(
                    name, desc, 1, 0, 0, 0, 0, 0);
            }
            D_000469D8 = cType::InitializeType(
                0, 0, 0x232, D_000385DC, 0, 0, 0, 0);
        }
        __asm__ volatile("" ::: "memory");
        const cType *parentType = D_000469D8;
        cBase *(*factory)(cMemPool *, cBase *) =
            (cBase *(*)(cMemPool *, cBase *))0x208DDC;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046BB4 = cType::InitializeType(
            0, 0, 0x238, parentType, factory, 0, 0, 0);
    }
    return D_00046BB4;
}

const cType *eSimulatedController::GetType(void) const {
    if (D_00046BD4 == 0) {
        if (D_000469D8 == 0) {
            if (D_000385DC == 0) {
                const char *name = (const char *)0x36CD74;
                const char *desc = (const char *)0x36CD7C;
                __asm__ volatile("" : "+r"(name), "+r"(desc));
                D_000385DC = cType::InitializeType(
                    name, desc, 1, 0, 0, 0, 0, 0);
            }
            D_000469D8 = cType::InitializeType(
                0, 0, 0x232, D_000385DC, 0, 0, 0, 0);
        }
        D_00046BD4 = cType::InitializeType(
            0, 0, 0x233, D_000469D8, 0, 0, 0, 0);
    }
    return D_00046BD4;
}

const cType *eSimulatedControllerConfig::GetType(void) const {
    if (D_00046BF8 == 0) {
        if (D_000469E8 == 0) {
            if (D_000385DC == 0) {
                const char *name = (const char *)0x36CD74;
                const char *desc = (const char *)0x36CD7C;
                __asm__ volatile("" : "+r"(name), "+r"(desc));
                D_000385DC = cType::InitializeType(
                    name, desc, 1, 0, 0, 0, 0, 0);
            }
            D_000469E8 = cType::InitializeType(
                0, 0, 0x23D, D_000385DC, 0, 0, 0, 0);
        }
        D_00046BF8 = cType::InitializeType(
            0, 0, 0x23E, D_000469E8, 0, 0, 0, 0);
    }
    return D_00046BF8;
}
