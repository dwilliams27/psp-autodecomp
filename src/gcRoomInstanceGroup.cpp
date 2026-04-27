// gcRoomInstanceGroup — inherits cGroup. Decompiled functions:
//   0x000d2c20 gcRoomInstanceGroup::Write(cFile &) const
//   0x00238534 gcRoomInstanceGroup::IsManagedTypeExternal(void) const
//   0x00238550 gcRoomInstanceGroup::~gcRoomInstanceGroup(void)
// Pattern mirrors gcTableTemplateGroup.cpp / eTextureGroup.cpp.

class cBase;
class cFile;
class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cGroup {
public:
    cBase *m_parent;       // 0x00
    void *m_classdesc;     // 0x04

    ~cGroup();
    void Write(cFile &) const;
};

class gcRoomInstanceGroup : public cGroup {
public:
    void Write(cFile &) const;
    bool IsManagedTypeExternal() const;
    static bool IsManagedTypeExternalStatic();
    ~gcRoomInstanceGroup();

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        char *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
};

// ============================================================
// 0x000d2c20 — Write(cFile &) const
// ============================================================
void gcRoomInstanceGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}

// ============================================================
// 0x00238534 — IsManagedTypeExternal(void) const
// ============================================================
bool gcRoomInstanceGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

// ============================================================
// 0x00238550 — ~gcRoomInstanceGroup(void)
// ============================================================
gcRoomInstanceGroup::~gcRoomInstanceGroup() {
    *(void **)((char *)this + 4) = (void *)0x386990;
}
