#ifndef GCUIEDITBOX_H
#define GCUIEDITBOX_H

#include "gcUIWidget.h"
#include "gcCinematic.h"  // for cHandleT

class eSprite;
class cBase;
class cTimeValue;
class gcEventStackData;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct gcUIEditBoxDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcUITextControl {
public:
    char pad_text_control[0x110];

    gcUITextControl(cBase *);
    ~gcUITextControl();
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

class gcUIEditBox : public gcUITextControl {
public:
    unsigned int mEditFlags;           // 0x110, bit 0x80000000 = cursor visible
    cHandleT<eSprite> mCursorSprite;   // 0x114
    char pad_118[0x4];                 // 0x118 (unknown)
    int mCursorPos;                    // 0x11C

    gcUIEditBox(cBase *);
    ~gcUIEditBox();
    static cBase *New(cMemPool *, cBase *);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        gcUIEditBoxDeleteRecord *rec =
            (gcUIEditBoxDeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }

    const cType *GetType(void) const;
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
