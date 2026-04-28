// Days of Thunder decompilation: gcValEntityHasConstants
//   0x0032f92c AssignCopy(const cBase *)
//   0x0032fb98 Write(cFile &) const

#include "cBase.h"

class cFile;
class cMemPool;
class cWriteBlock;

class cHandle {
public:
    int mId;
    void Write(cWriteBlock &) const;
};

class cWriteBlock {
public:
    cFile *file;
    unsigned int _pos;
    cWriteBlock(cFile &, unsigned int);
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

class gcValEntityHasConstants {
public:
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
};

template <class T> T *dcast(const cBase *);
void gcLValue_Write(const gcValEntityHasConstants *, cFile &);

// 0x0032f92c (80B) — AssignCopy
void gcValEntityHasConstants::AssignCopy(const cBase *base) {
    gcValEntityHasConstants *other = dcast<gcValEntityHasConstants>(base);
    gcDesiredEntity *srcptr = (gcDesiredEntity *)(other + 8);
    ((gcDesiredEntity *)((char *)this + 8))->operator=(*srcptr);
    *(cHandle *)((char *)this + 52) = *(const cHandle *)((char *)other + 52);
}

// 0x0032fb98 (120B) — Write
void gcValEntityHasConstants::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcLValue_Write(this, file);
    const cTypeMethod *e =
        (const cTypeMethod *)((char *)((const gcDesiredObject *)((const char *)this + 8))->mType + 40);
    char *base = (char *)this + 8;
    typedef void (*WriteFn)(void *, cFile *);
    ((WriteFn)e->fn)(base + e->offset, wb.file);
    ((const cHandle *)((const char *)this + 52))->Write(wb);
    wb.End();
}
