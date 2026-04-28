// gcDesiredEntity.cpp — gcAll_psp.obj
//
// Functions in this file:
//   0x0011d67c gcDesiredEntity::Write(cFile &) const   120B
//   0x0026023c gcDesiredEntity::GetType(void) const    220B

class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    cFile *_file;       // +0
    int _pos;           // +4
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class gcDesiredEntityHelper {
public:
    void Write(cWriteBlock &) const;
};

class gcDesiredObject {
public:
    void Write(cFile &) const;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

extern cType *D_000385DC;     // shared cBase root type cache
extern cType *D_0009F3F4;     // shared gcDesiredObject type cache
extern cType *D_0009F3F8;     // gcDesiredEntity type cache

typedef void (*gcDesiredEntityWriteFn)(cBase *, cFile *);

struct gcDesiredEntityWriteSlot {
    short mOffset;                  // +0
    short _pad;                     // +2
    gcDesiredEntityWriteFn mWrite;  // +4
};

struct gcDesiredEntityTypeInfo2 {
    char _pad[0x28];                       // +0x00
    gcDesiredEntityWriteSlot mSlot;        // +0x28 — sub-object Write slot
};

class gcDesiredEntity {
public:
    void Write(cFile &) const;
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

// ── gcDesiredEntity::Write @ 0x0011d67c ──
void gcDesiredEntity::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    ((const gcDesiredObject *)this)->Write(file);
    ((const gcDesiredEntityHelper *)((char *)this + 12))->Write(wb);

    gcDesiredEntityTypeInfo2 *ti = *(gcDesiredEntityTypeInfo2 **)((char *)this + 24);
    gcDesiredEntityWriteSlot *slot = &ti->mSlot;
    cBase *embedded = (cBase *)((char *)this + 20);
    slot->mWrite((cBase *)((char *)embedded + slot->mOffset), wb._file);

    wb.End();
}

// ── gcDesiredEntity::GetType @ 0x0026023c ──
const cType *gcDesiredEntity::GetType(void) const {
    if (D_0009F3F8 == 0) {
        if (D_0009F3F4 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894, (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F3F4 = cType::InitializeType(0, 0, 0x12C, D_000385DC, 0, 0, 0, 0);
        }
        D_0009F3F8 = cType::InitializeType(0, 0, 0x12D, D_0009F3F4,
                                           (cBase *(*)(cMemPool *, cBase *))&gcDesiredEntity::New,
                                           0, 0, 0);
    }
    return D_0009F3F8;
}
