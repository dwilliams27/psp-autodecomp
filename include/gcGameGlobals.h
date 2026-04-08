#ifndef GCGAMEGLOBALS_H
#define GCGAMEGLOBALS_H

class gcGameGlobals {
public:
    char pad_000[0x54];
    int mGroups[1];

    void *GetGroups(int *);
};

#endif
