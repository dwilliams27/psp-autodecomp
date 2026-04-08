#ifndef ESHAPE_H
#define ESHAPE_H

class eBoxShape;
class eSphereShape;
class eMultiSphereShape;
class eCollisionContactInfo;
class mOCS;

class eShape {
public:
    int Collide(const eBoxShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eSphereShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eMultiSphereShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
};

#endif
