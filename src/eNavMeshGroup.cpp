class cBase;
class cFile;
class cMemPool;

template <class T> T *dcast(const cBase *);

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End();
};

class cGroup {
public:
    cBase *m_parent;        // 0x00
    void *m_vtbl;           // 0x04
    unsigned char mFlag;    // 0x08
    char _pad[3];
    int mField;             // 0x0C
    void Write(cFile &) const;
};

class eNavMeshGroup : public cGroup {
public:
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    bool IsManagedTypeExternal() const;
    static bool IsManagedTypeExternalStatic();
};

void eNavMeshGroup::AssignCopy(const cBase *base) {
    eNavMeshGroup *src = dcast<eNavMeshGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

bool eNavMeshGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

void eNavMeshGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}
