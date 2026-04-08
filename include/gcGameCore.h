#ifndef GCGAMECORE_H
#define GCGAMECORE_H

class cTimeValue {
public:
    float mTime;
};

class gcGameCore {
public:
    static void Update(cTimeValue, bool);
};

#endif
