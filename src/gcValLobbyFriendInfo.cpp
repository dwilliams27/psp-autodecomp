// gcValLobbyFriendInfo.cpp — decompiled from gcAll_psp.obj
//
// Class layout (size 0x14):
//   [0x00] cBase parent (cBase *)
//   [0x04] vtable
//   [0x08] int field_8
//   [0x0C] gcDesiredValue field_C
//   [0x10] int field_10

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

class gcDesiredValue {
public:
    int _value;
    void Write(cWriteBlock &) const;
};

class gcExpression {
public:
    void *_base[2];   // cBase parent + vtable
    void Write(cFile &) const;
};

class gcValue : public gcExpression {
public:
    void Write(cFile &) const;
};

class gcValLobbyFriendInfo : public gcValue {
public:
    int field_8;
    gcDesiredValue field_C;
    int field_10;

    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

extern char cBaseclassdesc[];                          // @ 0x37E6A8
extern char gcValLobbyFriendInfovirtualtable[];        // @ 0x18AB8 (linker reloc)

struct ePoolBlock {
    char  pad[0x1C];
    char *allocTable;
};

struct eAllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

// ── Write(cFile &) const  @ 0x00348030 ──
void gcValLobbyFriendInfo::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcValue::Write(file);
    wb.Write(field_8);
    wb.Write(field_10);
    field_C.Write(wb);
    wb.End();
}

// ── New(cMemPool *, cBase *) static  @ 0x00347e84 ──
cBase *gcValLobbyFriendInfo::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((ePoolBlock *)block)->allocTable;
    eAllocEntry *entry = (eAllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValLobbyFriendInfo *result = 0;
    gcValLobbyFriendInfo *obj = (gcValLobbyFriendInfo *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        *(void **)((char *)obj + 4) = cBaseclassdesc;
        *(cBase **)((char *)obj + 0) = parent;
        *(void **)((char *)obj + 4) = gcValLobbyFriendInfovirtualtable;
        *(int *)((char *)obj + 8) = 0;
        *(int *)((char *)obj + 0xC) = (int)obj | 1;
        *(int *)((char *)obj + 0x10) = 0;
        result = obj;
    }
    return (cBase *)result;
}
