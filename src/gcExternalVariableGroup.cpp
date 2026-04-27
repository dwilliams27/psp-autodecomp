// gcExternalVariableGroup — inherits cGroup. Mirrors gcTableTemplateGroup.cpp.
// Decompiled functions:
//   0x000d1510 gcExternalVariableGroup::Write(cFile &) const
//   0x00237b94 gcExternalVariableGroup::IsManagedTypeExternal(void) const
//   0x00237bb0 gcExternalVariableGroup::~gcExternalVariableGroup(void)

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

class gcExternalVariableGroup : public cGroup {
public:
    unsigned char mFlag;   // 0x08
    char _pad1[3];         // 0x09..0x0B
    int mField;            // 0x0C

    void Write(cFile &) const;
    bool IsManagedTypeExternal() const;
    static bool IsManagedTypeExternalStatic();
    ~gcExternalVariableGroup();

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
// 0x000d1510 — Write(cFile &) const
// ============================================================
void gcExternalVariableGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}

// ============================================================
// 0x00237b94 — IsManagedTypeExternal(void) const
// ============================================================
bool gcExternalVariableGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

// ============================================================
// 0x00237bb0 — ~gcExternalVariableGroup(void)
// ============================================================
gcExternalVariableGroup::~gcExternalVariableGroup() {
    *(void **)((char *)this + 4) = (void *)0x386750;
}
