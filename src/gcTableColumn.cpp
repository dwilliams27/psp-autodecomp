class cOutStream;
class cInStream;

struct gcTableColumn {
    float Get(int) const;
    void *GetContainer(void) const;
    void Write(cOutStream &) const;
    void Read(cInStream &);
};

struct gcTableColumnStringTable {
    void Set(int, float);
};

float gcTableColumn::Get(int) const {
    return 0.0f;
}

void *gcTableColumn::GetContainer(void) const {
    return 0;
}

void gcTableColumn::Write(cOutStream &) const {
}

void gcTableColumn::Read(cInStream &) {
}

void gcTableColumnStringTable::Set(int, float) {
}
