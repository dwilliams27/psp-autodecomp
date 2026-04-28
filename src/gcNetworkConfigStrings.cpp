// gcNetworkConfigStrings — gcAll_psp.obj
//   0x00285de8 New(cMemPool *, cBase *) static            (144B)
//   0x00285f54 Write(cFile &) const                       (100B)
//
// Layout: derives from gcStringValue, embeds gcDesiredValue at offset 0x8,
// has int member at 0xC. Total size 0x10. Type ID for Write block = 10.

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

extern char gcNetworkConfigStringsvirtualtable[];

class gcStringValue {
public:
    void Write(cFile &) const;
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcNetworkConfigStrings : public gcStringValue {
public:
    void Write(cFile &) const;
    static gcNetworkConfigStrings *New(cMemPool *, cBase *);
};

// 0x00285f54 — Write(cFile &) const
void gcNetworkConfigStrings::Write(cFile &file) const {
    cWriteBlock wb(file, 10);
    gcStringValue::Write(file);
    wb.Write(*(const int *)((const char *)this + 0xC));
    ((const gcDesiredValue *)((const char *)this + 8))->Write(wb);
    wb.End();
}

// 0x00285de8 — New(cMemPool *, cBase *) static
struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

gcNetworkConfigStrings *gcNetworkConfigStrings::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    void *base = (char *)block + entry->offset;
    gcNetworkConfigStrings *result = 0;
    int *p = (int *)entry->fn(base, 0x10, 4, 0, 0);
    if (p != 0) {
        *(int *)((char *)p + 4)  = 0x37E6A8;
        *(cBase **)((char *)p + 0) = parent;
        *(int *)((char *)p + 4)  = (int)gcNetworkConfigStringsvirtualtable;
        *(int *)((char *)p + 8)  = (int)p | 1;
        *(int *)((char *)p + 0xC) = 0;
        result = (gcNetworkConfigStrings *)p;
    }
    return result;
}
