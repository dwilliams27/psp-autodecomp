#ifndef EMEMCARD_H
#define EMEMCARD_H

class eMemCard {
public:
    struct eLoadParams {};
    struct eSaveParams {};

    static int Reset(void);
    static void EndLoad(const eLoadParams &);
    static void EndSave(const eSaveParams &);
};

#endif
