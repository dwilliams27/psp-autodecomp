#ifndef GCUIWIDGET_H
#define GCUIWIDGET_H

class mVec2;

class gcUIWidget {
public:
    enum gcUIEffect {};
    void OnGetSize(mVec2 *, mVec2 *, unsigned int) const;
};

#endif
