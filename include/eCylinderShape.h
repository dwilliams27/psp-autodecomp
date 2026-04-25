#ifndef ECYLINDERSHAPE_H
#define ECYLINDERSHAPE_H

class cBase;
class cFile;
class cMemPool;
class cType;
struct mVec3;
struct mOCS;
class eShape;
class eCollisionContactInfo;

class eCylinderShape {
public:
    char _pad[0x80];
    float mRadius;       // offset 0x80
    float mHalfHeight;   // offset 0x84
    int _unk88;          // offset 0x88

    eCylinderShape(cBase *);
    ~eCylinderShape();

    void Write(cFile &) const;
    void GetInertialTensor(float mass, mVec3 *out) const;
    int Collide(const eShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int NeedsRollingFriction(float *) const;
    void AssignCopy(const cBase *);
    void GetProjectedMinMax(const mVec3 &, const mOCS &, float *, float *) const;
    const cType *GetType(void) const;

    static eCylinderShape *New(cMemPool *, cBase *);
};

#endif
