#ifndef ECYLINDERSHAPE_H
#define ECYLINDERSHAPE_H

class cBase;
class cFile;
class cMemPool;
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

    void Write(cFile &) const;
    void GetInertialTensor(float mass, mVec3 *out) const;
    int Collide(const eShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;

    static eCylinderShape *New(cMemPool *, cBase *);
};

#endif
