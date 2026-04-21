#include "gcValFPSInfo.h"
#include "cBase.h"

class cMemPool;

void cStrAppend(char *, const char *, ...);
gcValFPSInfo *dcast(const cBase *);
void *cMemPool_GetPoolFromPtr(const void *);

extern const char gcValFPSInfo_text[];
extern char gcValFPSInfovirtualtable[];

extern void *g_fps_pInstance;
extern int g_fps_x;
extern int g_fps_y;
extern float g_fps_rate;

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

// -----------------------------------------------------------------------------
// Function: gcValFPSInfo::VisitReferences(...)
// -----------------------------------------------------------------------------
void gcValFPSInfo::VisitReferences(unsigned int arg0, cBase *arg1,
    void (*arg2)(cBase *, unsigned int, void *),
    void *arg3, unsigned int arg4) {
    if (arg2 != 0) {
        arg2(arg1, (unsigned int)this, arg3);
    }
}

// -----------------------------------------------------------------------------
// Function: gcValFPSInfo::AssignCopy(const cBase *)
// -----------------------------------------------------------------------------
void gcValFPSInfo::AssignCopy(const cBase *base) {
    gcValFPSInfo *other = dcast(base);
    this->f8 = other->f8;
}

// -----------------------------------------------------------------------------
// Function: gcValFPSInfo::GetText(char *) const
// -----------------------------------------------------------------------------
void gcValFPSInfo::GetText(char *buf) const {
    cStrAppend(buf, gcValFPSInfo_text);
}

// -----------------------------------------------------------------------------
// Function: gcValFPSInfo::Evaluate(void) const
// -----------------------------------------------------------------------------
float gcValFPSInfo::Evaluate(void) const {
    void *inst = g_fps_pInstance;
    void *obj = 0;
    if (inst != 0) {
        obj = ((void **)inst)[4];
    }
    if (obj != 0) {
        *(float *)((char *)obj + 0x18) = (float)g_fps_x + (float)g_fps_y;
        *(float *)((char *)obj + 0x1C) = (float)g_fps_x;
        *(float *)((char *)obj + 0x20) = (float)g_fps_y;
    }
    return g_fps_rate;
}

// -----------------------------------------------------------------------------
// Function: gcValFPSInfo::~gcValFPSInfo(void)
// -----------------------------------------------------------------------------
extern "C" {

void gcValFPSInfo___dtor_gcValFPSInfo_void(gcValFPSInfo *self, int flags) {
    if (self != 0) {
        ((void **)self)[1] = gcValFPSInfovirtualtable;
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
