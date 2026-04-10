#ifndef GCUIWIDGET_H
#define GCUIWIDGET_H

class mVec2;
class gcUIDialog;

class gcUIWidget {
public:
    char pad_000[0x20];
    gcUIDialog *mpDialog;       // 0x20
    unsigned int mFlags;        // 0x24

    enum gcUIEffect {};
    void OnGetSize(mVec2 *, mVec2 *, unsigned int) const;
    void *GetResizableSprites(void);
    bool NeedsEffectUpdate(void) const;
    void InsertIntoDialog(gcUIDialog *);
    void InitialUpdate(void);
    void InitialUpdateUI(void);
    void CaptureFocus(bool);
    int IsUpdateEmpty(bool, bool) const;
};

#endif
