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
extern char cBaseclassdesc[];                           // @ 0x37E6A8

gcValCameraValue *dcast(const cBase *);
void cStrAppend(char *, const char *, ...);
void gcValue_Write(const gcValCameraValue *, cFile &);

extern const char gcValCameraValue_text[];

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
//
// Canonical C++ destructor. SNC's ABI auto-emits the (this != 0) guard, the
// absence of a parent-destructor chain, and the deleting-tail dispatch through
// operator delete on (flag & 1). Body just resets the classdesc pointer at
// offset 4 to the parent (cBase) classdesc.
// -----------------------------------------------------------------------------
gcValCameraValue::~gcValCameraValue(void) {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}
