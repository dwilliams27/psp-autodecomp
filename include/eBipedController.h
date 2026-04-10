#ifndef EBIPEDCONTROLLER_H
#define EBIPEDCONTROLLER_H

class mVec3;

class eBipedController {
public:
    void ApplyForce(int, const mVec3 &, const mVec3 &);
    void ApplyPositionedImpulse(int, const mVec3 &, const mVec3 &);
    void ApplyPositionedForce(int, const mVec3 &, const mVec3 &);
    int GetCollisionMask(void) const;
    void OnSnappedTo(void);

    char _pad[0x114];
    int collisionMask;
};

#endif
