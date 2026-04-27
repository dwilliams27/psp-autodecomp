// gcEnumerationEntry.cpp — decompiled from gcAll_psp.obj.
// Parent: cNamed. cNamed parent's classdesc lives at 0x37E6A8 (cBaseclassdesc).
//
// Functions:
//   0x000d3d6c gcEnumerationEntry::Write(cFile &) const                100B
//   0x00238a9c gcEnumerationEntry::AssignCopy(const cBase *)           120B
//   0x00238ca0 gcEnumerationEntry::~gcEnumerationEntry(void)           100B

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned int);
    void Write(unsigned char);
    void End(void);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct cNameData { int _w[6]; };  // 24-byte name buffer at cNamed+8

class cNamed {
public:
    char _pad[8];          // cBase header: parent (0), classdesc (4)
    cNameData mName;       // 8..0x20
    cNamed(cBase *);
    void Write(cFile &) const;
};

struct cHandle {
    int mIndex;
};

extern char cBaseclassdesc[];   // @ 0x37E6A8

template <class T> T *dcast(const cBase *);

class gcEnumerationEntry : public cNamed {
public:
    unsigned char mField20;  // 0x20
    unsigned int  mField24;  // 0x24

    gcEnumerationEntry(cBase *);
    ~gcEnumerationEntry(void);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

// ── gcEnumerationEntry::Write @ 0x000d3d6c ──
void gcEnumerationEntry::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    cNamed::Write(file);
    wb.Write(mField24);
    wb.Write(mField20);
    wb.End();
}

// ── gcEnumerationEntry::AssignCopy @ 0x00238a9c ──
void gcEnumerationEntry::AssignCopy(const cBase *base) {
    gcEnumerationEntry *other = dcast<gcEnumerationEntry>(base);
    cNameData *src = (cNameData *)((char *)other + 8);
    cNameData *dst = (cNameData *)((char *)this + 8);
    *dst = *src;
    *(unsigned char *)((char *)this + 0x20) = *(const unsigned char *)((char *)other + 0x20);
    *(cHandle *)((char *)this + 0x24) = *(const cHandle *)((char *)other + 0x24);
}

// ── gcEnumerationEntry::~gcEnumerationEntry @ 0x00238ca0 ──
// SNC auto-emits the (this!=0) guard and the (flag & 1) deleting tail
// dispatch through operator delete. The body just restores the parent
// (cNamed→cBase) classdesc at offset 4.
gcEnumerationEntry::~gcEnumerationEntry(void) {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}
