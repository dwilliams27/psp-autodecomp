#ifndef GCUIDIALOG_H
#define GCUIDIALOG_H

class gcUI;
class cMemPool;
class gcUIWidgetGroup;
class gcDoUIShowDialog;

class gcUIWidget;

class gcUIDialog {
public:
    char pad_000[0x44];
    gcUI *mpUI;                    // 0x44
    char pad_048[0x08];
    int mFlags;                    // 0x50
    char pad_054[0x1E4];
    int mEvent0;                   // 0x238
    int mEvent1;                   // 0x23C
    int mEvent2;                   // 0x240
    int mEvent3;                   // 0x244
    int mEvent4;                   // 0x248

    void CalcInstanceSize(void);
    void OnFinalOpen(void);
    void ClearEvents(void);
    void Reset(cMemPool *pool, bool flag);
    void Focus(void);
    void Close(void);
    void CreateControl(gcUIWidget *);
};

#endif
