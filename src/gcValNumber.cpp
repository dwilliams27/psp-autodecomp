#include "gcValNumber.h"
#include "cBase.h"

class cFile;

// -----------------------------------------------------------------------------
// Forward decls for external support code
// -----------------------------------------------------------------------------

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(float);
    void End(void);
};

class cStr {
public:
    char _data[256];
    cStr(const char *fmt, ...);
    operator const char *() const { return _data; }
};

void cStrCat(char *, const char *);
void gcValue_Write(const gcValNumber *, cFile &);
gcValNumber *dcast(const cBase *);
void *cMemPool_GetPoolFromPtr(const void *);

extern const char gcValNumber_fmt[];
extern char gcValNumbervirtualtable[];

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

// -----------------------------------------------------------------------------
// Function 1: gcValNumber::gcValNumber(cBase *, float)
// -----------------------------------------------------------------------------
gcValNumber::gcValNumber(cBase *base, float val) {
    this->pad0 = (int)base;
    this->pad4 = 0x38A468;
    this->mValue = val;
}

// -----------------------------------------------------------------------------
// Function 2: gcValNumber::AssignCopy(const cBase *)
// -----------------------------------------------------------------------------
void gcValNumber::AssignCopy(const cBase *base) {
    gcValNumber *other = dcast(base);
    this->mValue = other->mValue;
}

// -----------------------------------------------------------------------------
// Function 3: gcValNumber::Write(cFile &) const
// -----------------------------------------------------------------------------
void gcValNumber::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcValue_Write(this, file);
    wb.Write(this->mValue);
    wb.End();
}

// -----------------------------------------------------------------------------
// Function 4: gcValNumber::GetText(char *) const
// -----------------------------------------------------------------------------
void gcValNumber::GetText(char *buf) const {
    cStrCat(buf, cStr(gcValNumber_fmt, (double)this->mValue));
}

// -----------------------------------------------------------------------------
// Function 5: gcValNumber::~gcValNumber(void)
// -----------------------------------------------------------------------------
extern "C" {

void gcValNumber___dtor_gcValNumber_void(gcValNumber *self, int flags) {
    if (self != 0) {
        ((void **)self)[1] = gcValNumbervirtualtable;
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
