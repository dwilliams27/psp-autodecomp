#ifndef GCUICONTROL_H
#define GCUICONTROL_H

struct mVec2;
struct eCamera;
struct mOCS;
struct cBase;
struct cFile;

struct gcUIControl {
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    void OnDraw(const eCamera &, const mOCS &, const mVec2 &, const mVec2 &, float, float, float, unsigned int);
    void GetAdditionalDrawSize(mVec2 *, unsigned int) const;
};

#endif
