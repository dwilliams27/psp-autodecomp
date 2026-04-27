// eStaticGeom.cpp — eAll_psp.obj
//
// Functions:
//   eStaticGeom::Write(cFile &) const           @ 0x00040D28  (76B)
//   eStaticGeom::GetType(void) const            @ 0x001EBBB4 (212B)
//   eStaticGeom::~eStaticGeom(void)             @ 0x001EBC88 (152B)
//
// eStaticGeom inherits from ePortal — the dtor stores ePortalvirtualtable+0x30
// at offset 4 (the secondary-vtable thunk for the eStaticGeom subobject), then
// chains to eGeom::~eGeom (eGeom is the further-base in the diamond), then on
// the deleting-flag path looks up the per-pool deleter or falls back to free.

class cBase;
class cFile;
class cMemPool;
class cType;
class eGeom;
class eStaticGeom;

extern char ePortalvirtualtable[];      // 0x380B18 — vtable used by eStaticGeom

extern const char eStaticGeom_typeName[];  // 0x36CD74 — "cBase"
extern const char eStaticGeom_typeDesc[];  // 0x36CD7C — "Base"

extern "C" {
void *cMemPool_GetPoolFromPtr(const void *);
void eGeom___dtor_eGeom_void(void *self, int flags);
void free(void *);
}

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class eGeom {
public:
    void Write(cFile &) const;
};

class eStaticGeom : public eGeom {
public:
    void Write(cFile &) const;
    const cType *GetType(void) const;
};

// Static type-cache slots — addresses 0x385DC, 0x40FF4, 0x469A4 in the binary.
// Relocation masking in compare_func handles the fact that our linker assigns
// distinct addresses; the byte stream after masking is identical.
static cType *type_eStaticGeom_base;
static cType *type_eStaticGeom_mid;
static cType *type_eStaticGeom;

// ── eStaticGeom::Write(cFile &) const ──
void eStaticGeom::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const eGeom *)this)->Write(file);
    wb.End();
}

// ── eStaticGeom::GetType(void) const ──
const cType *eStaticGeom::GetType(void) const {
    if (!type_eStaticGeom) {
        if (!type_eStaticGeom_mid) {
            if (!type_eStaticGeom_base) {
                type_eStaticGeom_base = cType::InitializeType(
                    eStaticGeom_typeName, eStaticGeom_typeDesc, 1, 0, 0, 0, 0, 0);
            }
            type_eStaticGeom_mid = cType::InitializeType(
                0, 0, 0x16, type_eStaticGeom_base, 0, 0, 0, 0);
        }
        type_eStaticGeom = cType::InitializeType(
            0, 0, 0x1A, type_eStaticGeom_mid, 0, 0, 0, 0);
    }
    return type_eStaticGeom;
}

// ── eStaticGeom::~eStaticGeom(void) ──
struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

extern "C" void eStaticGeom___dtor_eStaticGeom_void(eStaticGeom *self, int flags) {
    if (self != 0) {
        ((void **)self)[1] = ePortalvirtualtable;
        eGeom___dtor_eGeom_void(self, 0);
        if (flags & 1) {
            void *pool = cMemPool_GetPoolFromPtr(self);
            if (pool != 0) {
                void *block = *(void **)((char *)pool + 0x24);
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
