#ifndef EKEYFRAMEDCONTROLLER_H
#define EKEYFRAMEDCONTROLLER_H

class cMemPool;
class cFile;
class eDynamicModel;
class mVec3;
class mOCS;

class eKeyframedController {
public:
    int Read(cFile &, cMemPool *);
    void Activate(eDynamicModel *);
    void Deactivate(eDynamicModel *);
    void __attribute__((naked)) GetVelocity(int, const mVec3 &, mVec3 *) const;
    void SetPosition(int, const mVec3 &);
    void ApplyImpulse(int, const mVec3 &, const mVec3 &);
    void ApplyForce(int, const mVec3 &, const mVec3 &);
    void ApplyPositionedImpulse(int, const mVec3 &, const mVec3 &);
    void ApplyPositionedForce(int, const mVec3 &, const mVec3 &);
    void Reset(cMemPool *, bool);
    void PostUpdate(mOCS *, eDynamicModel *) const;
    void AssignCopy(const class cBase *);
};

#endif
