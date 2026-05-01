#ifndef ECYLINDERSHAPE_H
#define ECYLINDERSHAPE_H

class cBase;
class cFile;
class cMemPool;
class cType;
struct mVec3;
struct mOCS;
class mRay;
class mSphere;
class mCollideInfo;
class eCollisionInfo;
class eShape;
class eCollisionContactInfo;
class eContactCollector;

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
    float GetVolume(void) const;
    int CastRay(const mRay &, const eCollisionInfo &, mVec3 *, mVec3 *, float *) const;
    int CastSphere(const mSphere &, const mCollideInfo &, const eCollisionInfo &, mVec3 *, mVec3 *, float *) const;
    int GetSweptContacts(int, const mSphere *, const mCollideInfo *, const eCollisionInfo &, eContactCollector *) const;
    int GetEmbedContacts(int, const mSphere *, const eCollisionInfo &, eContactCollector *) const;
    int Collide(const eShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int NeedsRollingFriction(float *) const;
    void AssignCopy(const cBase *);
    void GetProjectedMinMax(const mVec3 &, const mOCS &, float *, float *) const;
    const cType *GetType(void) const;

    static eCylinderShape *New(cMemPool *, cBase *);
};

#endif
