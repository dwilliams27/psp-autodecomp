#ifndef GCUIMARQUEECONTROL_H
#define GCUIMARQUEECONTROL_H

#include "gcUIWidget.h"

class cTimeValue {
public:
    int mTime;
};

class gcUIMarqueeControl : public gcUIWidget {
public:
    void Update(cTimeValue, const gcEventStackData &);
};

#endif
