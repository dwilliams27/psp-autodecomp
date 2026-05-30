// gcTableColumnByte::AssignCopy(const cBase *)
// 0x00270f30, 52B, gcAll_psp.obj
// Mirrors verified-matched sibling gcTableColumnShort::AssignCopy (0x00271504).
class cBase;

class cArrayByte {
public:
    unsigned char *mData;
    cArrayByte &operator=(const cArrayByte &);
};

struct gcTableColumn {
    void *mOwner;
    void *mClassDesc;
};

struct gcTableColumnByte : public gcTableColumn {
    cArrayByte mValues;

    void AssignCopy(const cBase *other);
};

gcTableColumnByte *dcast(const cBase *);

// 0x00270f30, 52B
void gcTableColumnByte::AssignCopy(const cBase *other) {
    gcTableColumnByte *src = dcast(other);
    cArrayByte &srcArr = *(cArrayByte *)((char *)src + 8);
    ((cArrayByte *)((char *)this + 8))->operator=(srcArr);
}
