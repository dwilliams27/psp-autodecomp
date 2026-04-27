// Days of Thunder decompilation: gcValEntityControllerVariable
//   0x003270f0 AssignCopy(const cBase *)
//   0x00327390 Write(cFile &) const
//   0x00328070 GetText(char *) const

#include "gcValEntityControllerVariable.h"
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

gcValEntityControllerVariable *dcast(const cBase *);
void cStrAppend(char *, const char *, ...);
void gcLValue_Write(const gcValEntityControllerVariable *, cFile &);

extern const char gcValEntityControllerVariable_text_fmt[];
extern const char gcValEntityControllerVariable_text_arg[];

// 0x003270f0 (72B) — AssignCopy
void gcValEntityControllerVariable::AssignCopy(const cBase *base) {
    gcValEntityControllerVariable *other = dcast(base);
    gcDesiredEntity *srcptr = (gcDesiredEntity *)(other + 8);
    ((gcDesiredEntity *)((char *)this + 8))->operator=(*srcptr);
    *(int *)((char *)this + 52) = *(const int *)((char *)other + 52);
}

// 0x00327390 (120B) — Write
void gcValEntityControllerVariable::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcLValue_Write(this, file);
    const cTypeMethod *e =
        (const cTypeMethod *)((char *)((const gcDesiredObject *)((const char *)this + 8))->mType + 40);
    char *base = (char *)this + 8;
    typedef void (*WriteFn)(void *, cFile *);
    ((WriteFn)e->fn)(base + e->offset, wb.file);
    wb.Write(*(const int *)((const char *)this + 52));
    wb.End();
}

// 0x00328070 (88B) — GetText
void gcValEntityControllerVariable::GetText(char *buf) const {
    const cTypeMethod *e =
        (const cTypeMethod *)((char *)((const gcDesiredObject *)((const char *)this + 8))->mType + 120);
    char *base = (char *)this + 8;
    typedef void (*TextFn)(void *, char *);
    ((TextFn)e->fn)(base + e->offset, buf);
    cStrAppend(buf,
               gcValEntityControllerVariable_text_fmt,
               gcValEntityControllerVariable_text_arg);
}
