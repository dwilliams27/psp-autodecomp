#ifndef EMEMCARD_H
#define EMEMCARD_H

class eMemCard {
public:
    struct eLoadParams {};
    struct eSaveParams {};

    static int Reset(void);
    static void SetSlot(int);
    static float GetMinSaveTime(void);
    static float GetMinLoadTime(void);
    static void Format(void);
    static int GetSaveSize(int a, int b, int c);
    static void BeginLoad(const eLoadParams &);
    static void EndLoad(const eLoadParams &);
    static void EndSave(const eSaveParams &);
};

#endif
