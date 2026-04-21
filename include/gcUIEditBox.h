#ifndef GCUIEDITBOX_H
#define GCUIEDITBOX_H

#include "gcUIWidget.h"
#include "gcCinematic.h"  // for cHandleT

class eSprite;
class cTimeValue;
class gcEventStackData;

class gcUIEditBox : public gcUIWidget {
public:
    char pad_base[0x110 - 0x28];
    unsigned int mEditFlags;           // 0x110, bit 0x80000000 = cursor visible
    cHandleT<eSprite> mCursorSprite;   // 0x114
    char pad_118[0x4];                 // 0x118 (unknown)
    int mCursorPos;                    // 0x11C

    int GetCursor(cHandleT<eSprite> *) const;
    void DeltaCursorPos(int);
    void Backspace(void);
    void UpdateUI(cTimeValue, const gcEventStackData &);
    void Focus(bool, bool);

    void SetCursorPos(int);
    void DeleteString(int, int);
    void HandleBlink(cTimeValue);
};

#endif
