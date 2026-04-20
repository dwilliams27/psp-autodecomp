// Decompiled functions from gcAll_psp.obj
//   0x0013e50c gcPartialEntityControllerTemplate::gcPartialEntityControllerTemplate(cBase *)
//   0x002a0278 gcPartialEntityControllerTemplate::GetName(char *) const
//   0x0013e330 gcPartialEntityControllerTemplate::Write(cFile &) const
//   0x0013e530 gcPartialEntityControllerTemplate::~gcPartialEntityControllerTemplate(void)
//   0x002a01e0 gcPartialEntityControllerTemplate::GetType(void) const

#include "cBase.h"
#include "cFile.h"

class cMemPool {
public:
    static void *GetPoolFromPtr(const void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned int);
    void End(void);
};

class cHandle {
public:
    int mIndex;
    void Write(cWriteBlock &) const;
};

class cHandlePairT_gE_cSe {
public:
    const char *GetName(char *, bool, char *) const;
};

class gcPartialEntityControllerTemplate {
public:
    cBase *mOwner;          // 0x00
    void *mClassDesc;       // 0x04
    cHandle mHandle1;       // 0x08
    unsigned int mIndex;    // 0x0C
    cHandle mHandle2;       // 0x10

    gcPartialEntityControllerTemplate(cBase *);
    void GetName(char *) const;
    void Write(cFile &) const;
    const cType *GetType(void) const;
};

extern char gcPartialEntityControllerTemplateclassdesc[];
extern char gcPartialEntityControllerTemplatebasevtable[];
extern const char gcPartialEntityControllerTemplate_base_name[];
extern const char gcPartialEntityControllerTemplate_base_desc[];

static cType *type_base;
static cType *type_gcPartialEntityControllerTemplate;

// 0x0013e50c — constructor
gcPartialEntityControllerTemplate::gcPartialEntityControllerTemplate(cBase *owner) {
    mOwner = owner;
    mClassDesc = gcPartialEntityControllerTemplateclassdesc;
    mHandle1.mIndex = 0;
    mIndex = 0;
    mHandle2.mIndex = 0;
}

// 0x002a0278 — GetName forwards to cHandlePairT subobject at offset 8
void gcPartialEntityControllerTemplate::GetName(char *out) const {
    ((cHandlePairT_gE_cSe *)((char *)this + 8))->GetName(out, false, (char *)1);
}

// 0x0013e330 — Write
void gcPartialEntityControllerTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((cHandle *)((char *)this + 8))->Write(wb);
    wb.Write(((unsigned int *)this)[3]);
    ((cHandle *)((char *)this + 0x10))->Write(wb);
    wb.End();
}

// 0x0013e530 — destructor
struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

extern "C" void free(void *);

extern "C" void gcPartialEntityControllerTemplate___dtor_gcPartialEntityControllerTemplate_void(
        gcPartialEntityControllerTemplate *self, int flags) {
    if (self != 0) {
        self->mClassDesc = gcPartialEntityControllerTemplatebasevtable;
        if (flags & 1) {
            void *pool = cMemPool::GetPoolFromPtr(self);
            if (pool != 0) {
                void *block = *(void **)((char *)pool + 0x24);
                DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
                short off = rec->offset;
                rec->fn((char *)block + off, self);
            } else {
                free(self);
            }
        }
    }
}

// 0x002a01e0 — GetType
const cType *gcPartialEntityControllerTemplate::GetType(void) const {
    if (!type_gcPartialEntityControllerTemplate) {
        if (!type_base) {
            type_base = cType::InitializeType(gcPartialEntityControllerTemplate_base_name,
                                              gcPartialEntityControllerTemplate_base_desc,
                                              1, 0, 0, 0, 0, 0);
        }
        type_gcPartialEntityControllerTemplate = cType::InitializeType(0, 0, 0x104, type_base, 0, 0, 0, 0);
    }
    return type_gcPartialEntityControllerTemplate;
}
