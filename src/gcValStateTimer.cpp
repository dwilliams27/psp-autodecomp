#include "gcValStateTimer.h"
#include "cBase.h"

class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

extern char gcValStateTimervirtualtable[];

gcValStateTimer *dcast(const cBase *);
void cStrCat(char *, const char *);
void gcLValue_Write(const gcValStateTimer *, cFile &);
void *cMemPool_GetPoolFromPtr(void *);

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

// -----------------------------------------------------------------------------
// Function: gcValStateTimer::AssignCopy(const cBase *)
// -----------------------------------------------------------------------------
void gcValStateTimer::AssignCopy(const cBase *base) {
    dcast(base);
}

// -----------------------------------------------------------------------------
// Function: gcValStateTimer::Evaluate(void) const
// -----------------------------------------------------------------------------
float gcValStateTimer::Evaluate(void) const {
    char *p = *(char **)0x37D864;
    if (p == 0) {
        return 0.0f;
    }
    p += 24;
    return *(float *)0x36C800 * (float)*(int *)p;
}

// -----------------------------------------------------------------------------
// Function: gcValStateTimer::GetText(char *) const
// -----------------------------------------------------------------------------
void gcValStateTimer::GetText(char *buf) const {
    const char *text = (const char *)0x36F760;
    cStrCat(buf, text);
}

// -----------------------------------------------------------------------------
// Function: gcValStateTimer::Write(cFile &) const
// -----------------------------------------------------------------------------
void gcValStateTimer::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcLValue_Write(this, file);
    wb.End();
}

// -----------------------------------------------------------------------------
// Function: gcValStateTimer::~gcValStateTimer(void)
// -----------------------------------------------------------------------------
extern "C" {

void gcValStateTimer___dtor_gcValStateTimer_void(gcValStateTimer *self, int flags) {
    if (self != 0) {
        ((void **)self)[1] = gcValStateTimervirtualtable;
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
