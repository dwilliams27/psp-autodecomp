// Days of Thunder decompilation: gcValEntityPrimaryController
//   0x0033b0a0 AssignCopy(const cBase *)
//   0x0033b304 Write(cFile &) const
//   0x0033b530 GetText(char *) const

#include "gcValEntityPrimaryController.h"
#include "cBase.h"

class cFile;
class cMemPool;

class cWriteBlock {
public:
    cFile *file;
    unsigned int _pos;
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

struct cTypeMethod {
    short offset;
    short pad;
    void *fn;
};

class cType {
public:
    char _p0[0x28];
    cTypeMethod write_m;    // 0x28
    cTypeMethod read_m;     // 0x30
    char _p1[0x40];         // to 0x78
    cTypeMethod text_m;     // 0x78
};

class gcDesiredObject {
public:
    int _parent;
    cType *mType;
};

class gcDesiredEntity : public gcDesiredObject {
public:
    gcDesiredEntity &operator=(const gcDesiredEntity &);
};

gcValEntityPrimaryController *dcast(const cBase *);
void cStrAppend(char *, const char *, ...);
void gcLValue_Write(const gcValEntityPrimaryController *, cFile &);

extern const char gcValEntityPrimaryController_text_fmt[];
extern const char gcValEntityPrimaryController_text_arg[];
extern const char gcValEntityPrimaryController_text_suffix[];

// 0x0033b0a0 (72B) — AssignCopy
void gcValEntityPrimaryController::AssignCopy(const cBase *base) {
    gcValEntityPrimaryController *other = dcast(base);
    gcDesiredEntity *srcptr = (gcDesiredEntity *)(other + 8);
    ((gcDesiredEntity *)((char *)this + 8))->operator=(*srcptr);
    *(int *)((char *)this + 52) = *(const int *)((char *)other + 52);
}

// 0x0033b304 (120B) — Write
void gcValEntityPrimaryController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcLValue_Write(this, file);
    const cTypeMethod *e =
        (const cTypeMethod *)((char *)((const gcDesiredObject *)((const char *)this + 8))->mType + 40);
    char *base = (char *)this + 8;
    typedef void (*WriteFn)(void *, cFile *);
    ((WriteFn)e->fn)(base + e->offset, wb.file);
    wb.Write(*(const int *)((const char *)this + 52));
    wb.End();
}

// 0x0033b530 (104B) — GetText
void gcValEntityPrimaryController::GetText(char *buf) const {
    const cTypeMethod *e =
        (const cTypeMethod *)((char *)((const gcDesiredObject *)((const char *)this + 8))->mType + 120);
    char *base = (char *)this + 8;
    typedef void (*TextFn)(void *, char *);
    ((TextFn)e->fn)(base + e->offset, buf);
    cStrAppend(buf,
               gcValEntityPrimaryController_text_fmt,
               gcValEntityPrimaryController_text_arg);
    cStrAppend(buf, gcValEntityPrimaryController_text_suffix);
}
