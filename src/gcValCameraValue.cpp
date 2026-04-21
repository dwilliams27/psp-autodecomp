#include "gcValCameraValue.h"
#include "cBase.h"

class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

extern char gcValCameraValuevirtualtable[];

gcValCameraValue *dcast(const cBase *);
void cStrAppend(char *, const char *, ...);
void gcValue_Write(const gcValCameraValue *, cFile &);
void *cMemPool_GetPoolFromPtr(void *);

extern const char gcValCameraValue_text[];

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

// -----------------------------------------------------------------------------
// Function: gcValCameraValue::VisitReferences(...)
// -----------------------------------------------------------------------------
void gcValCameraValue::VisitReferences(unsigned int arg0, cBase *arg1,
    void (*arg2)(cBase *, unsigned int, void *),
    void *arg3, unsigned int arg4) {
    if (arg2 != 0) {
        arg2(arg1, (unsigned int)this, arg3);
    }
}

// -----------------------------------------------------------------------------
// Function: gcValCameraValue::AssignCopy(const cBase *)
// -----------------------------------------------------------------------------
void gcValCameraValue::AssignCopy(const cBase *base) {
    dcast(base);
}

// -----------------------------------------------------------------------------
// Function: gcValCameraValue::GetText(char *) const
// -----------------------------------------------------------------------------
void gcValCameraValue::GetText(char *buf) const {
    cStrAppend(buf, gcValCameraValue_text);
}

// -----------------------------------------------------------------------------
// Function: gcValCameraValue::Write(cFile &) const
// -----------------------------------------------------------------------------
void gcValCameraValue::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    gcValue_Write(this, file);
    wb.End();
}

// -----------------------------------------------------------------------------
// Function: gcValCameraValue::~gcValCameraValue(void)
// -----------------------------------------------------------------------------
extern "C" {

void gcValCameraValue___dtor_gcValCameraValue_void(gcValCameraValue *self, int flags) {
    if (self != 0) {
        ((void **)self)[1] = gcValCameraValuevirtualtable;
        if (flags & 1) {
            void *pool = cMemPool_GetPoolFromPtr(self);
            void *block = *(void **)((char *)pool + 0x24);
            DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
            short off = rec->offset;
            rec->fn((char *)block + off, self);
        }
    }
}

}
