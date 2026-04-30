#ifndef GCUIWIDGET_H
#define GCUIWIDGET_H

class mVec2;
class gcUIDialog;
class cTimeValue;
class gcEventStackData;
class cType;
class cBase;
class cFile;

class gcUIWidget {
public:
    char pad_000[0x20];
    gcUIDialog *mpDialog;       // 0x20
    unsigned int mFlags;        // 0x24

    enum gcUIEffect {};
    gcUIWidget(cBase *);
    ~gcUIWidget(void);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    gcUIDialog *GetParentDialog(void) const;
    void OnGetSize(mVec2 *, mVec2 *, unsigned int) const;
    void *GetResizableSprites(void);
    bool NeedsEffectUpdate(void) const;
    void InsertIntoDialog(gcUIDialog *);
    void InitialUpdate(void);
    void InitialUpdateUI(void);
    void CaptureFocus(bool);
    int IsUpdateEmpty(bool, bool) const;
    void UpdateUI(cTimeValue, const gcEventStackData &);
    void Focus(bool, bool);
};

#endif
