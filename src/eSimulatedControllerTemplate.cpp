// eSimulatedControllerTemplate.cpp — eAll_psp.obj
//
// Functions:
//   eSimulatedControllerTemplate::eSimulatedControllerTemplate(cBase *)
//                                                       @ 0x000766e8 (64B)
//   eSimulatedControllerTemplate::Write(cFile &) const  @ 0x00076510 (88B)
//   eSimulatedControllerTemplate::GetType(void) const   @ 0x0020e984 (212B)
//   eSimulatedControllerTemplate::~eSimulatedControllerTemplate(void)
//                                                       @ 0x00076728 (152B)
//
// eSimulatedControllerTemplate inherits from ePhysicsControllerTemplate.
// The dtor stores the parent vtable thunk (ePhysicsControllerTemplatevirtualtable
// area, 0x383ff8) at offset 4 then chains to ~ePhysicsControllerTemplate. The
// parent already runs the cBaseArray::RemoveAll cleanup; this dtor body is
// effectively empty before the chain (vtable swap + delete-tail only).
//
// Write is in the sched=1 zone (~0x06e000-0x0bab28); GetType lives outside
// that zone in the .text tail and uses the file-default sched=2.

#include "cBase.h"

class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(float);
    void End(void);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class ePhysicsControllerTemplate {
public:
    char _baseStorage[0x28];  // 40 bytes — full layout in ePhysicsControllerTemplate.cpp
    ePhysicsControllerTemplate(cBase *);
    void Write(cFile &) const;
    ~ePhysicsControllerTemplate(void);
};

class eSimulatedControllerTemplate : public ePhysicsControllerTemplate {
public:
    float mField28;           // +0x28 — initialized to 1.0f in ctor

    eSimulatedControllerTemplate(cBase *);
    ~eSimulatedControllerTemplate(void);
    void Write(cFile &) const;
    const cType *GetType(void) const;
};

extern char eSimulatedControllerTemplatevirtualtable[];

extern const char eSimulatedControllerTemplate_typeName[];  // 0x36CD74
extern const char eSimulatedControllerTemplate_typeDesc[];  // 0x36CD7C

// Static type-cache slots — relocation masking handles the actual addresses.
static cType *type_eSimulatedControllerTemplate_base;        // 0x385DC
static cType *type_eSimulatedControllerTemplate_mid;         // 0x469F4
static cType *type_eSimulatedControllerTemplate;             // 0x46C04

#pragma control sched=1
// ── eSimulatedControllerTemplate::eSimulatedControllerTemplate @ 0x000766e8 ──
eSimulatedControllerTemplate::eSimulatedControllerTemplate(cBase *parent)
    : ePhysicsControllerTemplate(parent)
{
    *(void **)((char *)this + 4) = eSimulatedControllerTemplatevirtualtable;
    __asm__ volatile(
        "lui $a0, 0x3f80\n"
        "mtc1 $a0, $f12\n"
        "swc1 $f12, 0x28(%0)\n"
        :: "r"(this) : "$a0", "$f12", "memory"
    );
}

// ── eSimulatedControllerTemplate::Write(cFile &) const @ 0x00076510 ──
void eSimulatedControllerTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 6);
    ((const ePhysicsControllerTemplate *)this)->Write(file);
    wb.Write(mField28);
    wb.End();
}
#pragma control sched=2

// ── eSimulatedControllerTemplate::GetType(void) const @ 0x0020e984 ──
#pragma control sched=1
const cType *eSimulatedControllerTemplate::GetType(void) const {
    if (type_eSimulatedControllerTemplate == 0) {
        if (type_eSimulatedControllerTemplate_mid == 0) {
            if (type_eSimulatedControllerTemplate_base == 0) {
                type_eSimulatedControllerTemplate_base = cType::InitializeType(
                    eSimulatedControllerTemplate_typeName,
                    eSimulatedControllerTemplate_typeDesc,
                    1, 0, 0, 0, 0, 0);
            }
            type_eSimulatedControllerTemplate_mid = cType::InitializeType(
                0, 0, 0x22F, type_eSimulatedControllerTemplate_base,
                0, 0, 0, 0);
        }
        type_eSimulatedControllerTemplate = cType::InitializeType(
            0, 0, 0x230, type_eSimulatedControllerTemplate_mid,
            0, 0, 0, 0);
    }
    return type_eSimulatedControllerTemplate;
}
#pragma control sched=2

// ── eSimulatedControllerTemplate::~eSimulatedControllerTemplate @ 0x00076728 ──
struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

extern "C" {
void *cMemPool_GetPoolFromPtr(const void *);
void ePhysicsControllerTemplate___dtor_ePhysicsControllerTemplate_void(void *, int);
void free(void *);
}

extern char ePhysicsControllerTemplate_secondary_vtable[];   // 0x383ff8

#pragma control sched=1
extern "C" void eSimulatedControllerTemplate___dtor_eSimulatedControllerTemplate_void(
    eSimulatedControllerTemplate *self, int flags)
{
    if (self != 0) {
        ((void **)self)[1] = ePhysicsControllerTemplate_secondary_vtable;
        ePhysicsControllerTemplate___dtor_ePhysicsControllerTemplate_void(self, 0);
        if (flags & 1) {
            void *pool = cMemPool_GetPoolFromPtr(self);
            if (pool != 0) {
                void *block = *(void **)((char *)pool + 0x24);
                __asm__ volatile("" ::: "memory");
                DeleteRecord *rec =
                    (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
                short off = rec->offset;
                rec->fn((char *)block + off, self);
            } else {
                free(self);
            }
        }
    }
}
#pragma control sched=2
