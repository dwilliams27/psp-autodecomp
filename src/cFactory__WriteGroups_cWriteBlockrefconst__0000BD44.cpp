// cFactory::WriteGroups(cWriteBlock &) const
// Address: 0x0000bd44, Obj: cAll_psp.obj
// Symbol: cFactory__WriteGroups_cWriteBlockrefconst__0000BD44

struct cGUID;

class cType {
public:
    int _pad0;
    unsigned int mTypeId;
};

class cFile;

class cWriteBlock {
public:
    cFile *mFile;
    unsigned int mOffset;

    void Write(bool);
    void Write(int);
    void Write(unsigned int);
};

class cFactory {
public:
    void WriteGroups(cWriteBlock &) const;
};

struct GroupArrayDispatchEntry {
    short offset;
    short _pad;
    void **(*fn)(void *, int *);
};

typedef void *(*cBaseGetTypeFn)(void *);
typedef void (*cBaseWriteFn)(void *, cFile *);

struct cBaseDispatchRecord {
    short offset;
    short pad;
    void *fn;
};

void cFactory::WriteGroups(cWriteBlock &wb) const {
    int count = 0;
    char *classdesc = *(char **)((char *)this + 4);
    GroupArrayDispatchEntry *entry = (GroupArrayDispatchEntry *)(classdesc + 168);
    void **groups = entry->fn((char *)this + entry->offset, &count);
    wb.Write(count);
    for (int i = 0; i < count; i++) {
        void *obj = groups[i];
        wb.Write((bool)(obj != 0));
        if (obj != 0) {
            cBaseDispatchRecord *te =
                (cBaseDispatchRecord *)(*(char **)((char *)obj + 4) + 8);
            cType *type =
                (cType *)((cBaseGetTypeFn)te->fn)((char *)obj + te->offset);
            wb.Write(type->mTypeId);
            cBaseDispatchRecord *we =
                (cBaseDispatchRecord *)(*(char **)((char *)obj + 4) + 0x28);
            ((cBaseWriteFn)we->fn)((char *)obj + we->offset, wb.mFile);
        }
    }
}
