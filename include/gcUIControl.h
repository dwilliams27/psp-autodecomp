#ifndef GCUICONTROL_H
#define GCUICONTROL_H

struct mVec2;
struct eCamera;
struct mOCS;

struct gcUIControl {
    void OnDraw(const eCamera &, const mOCS &, const mVec2 &, const mVec2 &, float, float, float, unsigned int);
    void GetAdditionalDrawSize(mVec2 *, unsigned int) const;
};

#endif
