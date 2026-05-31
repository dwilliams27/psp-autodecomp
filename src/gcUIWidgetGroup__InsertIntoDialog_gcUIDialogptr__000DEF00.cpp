// gcUIWidgetGroup::InsertIntoDialog(gcUIDialog *)
// Address: 0x000def00, Size: 252B
// Obj: gcAll_psp.obj

class gcUIDialog;

// Polymorphic base: 4 bytes of data then the vptr at +0x04 (SNC places the
// vptr after the introducing class's data members).
class gcUIWidgetGroupNodeBase {
public:
    char pad_00[0x04];                 // 0x00
public:                                // vptr at 0x04
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void OnInsertIntoDialog(gcUIDialog *);   // vtable +0x78
};

// Per-cell circular list node. Next link is +0x68.
class gcUIWidgetGroupNode : public gcUIWidgetGroupNodeBase {
public:
    char pad_08[0x60];                 // 0x08
    gcUIWidgetGroupNode *mNext;        // 0x68
};

// Base class supplies InsertIntoDialog (called first) at the start.
class gcUIWidget {
public:
    char pad_00[0x28];
    void InsertIntoDialog(gcUIDialog *);
};

class gcUIWidgetGroup : public gcUIWidget {
public:
    char pad_28[0x94];               // up to 0xBC
    gcUIWidgetGroupNode **mpCells;   // 0xBC
    int *mpRows;                     // 0xC0
    int *mpCols;                     // 0xC4
    void InsertIntoDialog(gcUIDialog *);
};

void gcUIWidgetGroup::InsertIntoDialog(gcUIDialog *dialog) {
    gcUIWidget::InsertIntoDialog(dialog);

    int numRows = 0;
    int *rows = mpRows;
    int *cols = mpCols;
    if (rows) {
        numRows = rows[-1] & 0x3FFFFFFF;
    }
    // NOTE: the permuter-discovered identifier "v90882" (the column count) must
    // be kept verbatim. SNC's local symbol ordering is sensitive to the name,
    // and this exact spelling is what reproduces the original delay-slot fill
    // (cols load in the rows-branch delay slot) and the `mult s2,a0` operand
    // order. Renaming it reintroduces a 2-byte register/scheduling diff.
    int v90882 = 0;
    if (cols) {
        v90882 = cols[-1] & 0x3FFFFFFF;
    }

    int total = v90882 * numRows;
    int i = 0;
    if (i < total) {
        int idx = 0;
        do {
            gcUIWidgetGroupNode *node = mpCells[idx];
            if (node) {
                do {
                    node->OnInsertIntoDialog(dialog);
                    node = node->mNext;
                } while (node != mpCells[idx]);
            }
            i += 1;
            idx += 1;
        } while (i < total);
    }
}
