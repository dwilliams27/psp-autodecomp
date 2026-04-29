// gcLanguageStrings — gcAll_psp.obj
//   0x0027f17c New(cMemPool *, cBase *) static            (140B)
//   0x0027f2e4 Write(cFile &) const                       (88B)
//   0x0027f33c Read(cFile &, cMemPool *)                  (200B)
//
// Layout: derives from gcStringValue, embeds gcDesiredValue at offset 0x8.
// Total size 0xC.

class cBase;
class cFile;
class cMemPool;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

void cFile_SetCurrentPos(void *, unsigned int);

extern char gcLanguageStringsvirtualtable[];
extern cType *D_000385DC;
extern cType *D_0009F454;
extern cType *D_0009F4E4;

class gcStringValue {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
};

class gcLanguageStrings : public gcStringValue {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    const cType *GetType(void) const;
    static gcLanguageStrings *New(cMemPool *, cBase *);
};

// 0x0027f208 — GetType(void) const
const cType *gcLanguageStrings::GetType(void) const {
    if (D_0009F4E4 == 0) {
        if (D_0009F454 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F454 = cType::InitializeType(0, 0, 0x170, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F4E4 = cType::InitializeType(
            0, 0, 0x1A9, D_0009F454,
            (cBase *(*)(cMemPool *, cBase *))&gcLanguageStrings::New, 0, 0, 0);
    }
    return D_0009F4E4;
}

// 0x0027f2e4 — Write(cFile &) const
void gcLanguageStrings::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcStringValue::Write(file);
    ((const gcDesiredValue *)((const char *)this + 8))->Write(wb);
    wb.End();
}

// 0x0027f33c — Read(cFile &, cMemPool *)
int gcLanguageStrings::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && gcStringValue::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    ((gcDesiredValue *)((char *)this + 8))->Read(rb);
    return result;
}

// 0x0027f17c — New(cMemPool *, cBase *) static
struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

gcLanguageStrings *gcLanguageStrings::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    void *base = (char *)block + entry->offset;
    gcLanguageStrings *result = 0;
    int *p = (int *)entry->fn(base, 0xC, 4, 0, 0);
    if (p != 0) {
        *(int *)((char *)p + 4)  = 0x37E6A8;
        *(cBase **)((char *)p + 0) = parent;
        *(int *)((char *)p + 4)  = (int)gcLanguageStringsvirtualtable;
        *(int *)((char *)p + 8)  = (int)p | 1;
        result = (gcLanguageStrings *)p;
    }
    return result;
}
