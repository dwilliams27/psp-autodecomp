#include "gcDoEntityForEach.h"
#include "cBase.h"

class cFile;

// -----------------------------------------------------------------------------
// External class / global declarations (byte-match via relocation masking)
// -----------------------------------------------------------------------------

namespace gcEvent {
    struct gcLoop {
        void *entity;
        int savedState;
        float f;
    };
}

class gcDoEntityFindAttachedBase {
public:
    void Write(cFile &) const;
    void GetText(char *, char *, bool) const;
    void Iterate(gcEvent::gcLoop &) const;
    void VisitReferences(unsigned int, cBase *,
                         void (*)(cBase *, unsigned int, void *),
                         void *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class gcExpressionList {
public:
    void Write(cWriteBlock &) const;
    void VisitReferences(unsigned int, cBase *,
                         void (*)(cBase *, unsigned int, void *),
                         void *, unsigned int);
};

struct gcEntityManager {
    char _pad[0x10];
    void *pCurrent;
};

extern gcEntityManager *g_entityManager;
extern char gcDoEntityForEachAttachedvirtualtable[];

extern "C" {
    void gcExpressionList___dtor(void *, int);
    void gcDoEntityFindAttachedBase___dtor(void *, int);
    void *cMemPool_GetPoolFromPtr(const void *);
}

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

// -----------------------------------------------------------------------------
// Function 1: gcDoEntityForEachAttached::GetText(char *) const
// -----------------------------------------------------------------------------
void gcDoEntityForEachAttached::GetText(char *buf) const {
    ((const gcDoEntityFindAttachedBase *)this)->GetText(buf, "FindAttachedEntity", true);
}

// -----------------------------------------------------------------------------
// Function 2: gcDoEntityForEachAttached::Write(cFile &) const
// -----------------------------------------------------------------------------
void gcDoEntityForEachAttached::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcDoEntityFindAttachedBase *)this)->Write(file);
    ((gcExpressionList *)((char *)this + 0x64))->Write(wb);
    wb.End();
}

// -----------------------------------------------------------------------------
// Function 3: gcDoEntityForEachAttached::Evaluate(void) const
// -----------------------------------------------------------------------------
float gcDoEntityForEachAttached::Evaluate(void) const {
    gcEvent::gcLoop loop;
    void *entity = g_entityManager ? g_entityManager->pCurrent : (void *)0;
    __asm__ volatile("" ::: "memory");
    loop.entity = entity;
    loop.f = 0.0f;
    loop.savedState = *(int *)((char *)entity + 0x3C);
    ((const gcDoEntityFindAttachedBase *)this)->Iterate(loop);
    int saved = loop.savedState;
    float ret = loop.f;
    *(int *)((char *)loop.entity + 0x3C) = saved;
    *(int *)loop.entity &= ~2;
    return ret;
}

// -----------------------------------------------------------------------------
// Function 4: gcDoEntityForEachAttached::VisitReferences(...)
// -----------------------------------------------------------------------------
void gcDoEntityForEachAttached::VisitReferences(unsigned int flags, cBase *ref,
    void (*visitor)(cBase *, unsigned int, void *),
    void *userData, unsigned int extra) {
    ((gcDoEntityFindAttachedBase *)this)->VisitReferences(flags, ref, visitor, userData, extra);
    gcExpressionList *list = (gcExpressionList *)((char *)this + 0x64);
    list->VisitReferences(flags, (cBase *)this, visitor, userData, extra);
}

// -----------------------------------------------------------------------------
// Function 5: gcDoEntityForEachAttached::~gcDoEntityForEachAttached(void)
// -----------------------------------------------------------------------------
extern "C" {

void gcDoEntityForEachAttached___dtor(gcDoEntityForEachAttached *self, int flags) {
    if (self != 0) {
        ((void **)self)[1] = gcDoEntityForEachAttachedvirtualtable;
        gcExpressionList___dtor((char *)self + 0x64, 2);
        gcDoEntityFindAttachedBase___dtor(self, 0);
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
