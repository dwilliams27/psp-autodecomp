#ifndef ECAPSULESHAPE_H
#define ECAPSULESHAPE_H

struct mVec3;
struct mOCS;

class eCapsuleShape {
public:
    char _pad[0x80];
    float radius;       // offset 0x80
    float halfHeight;   // offset 0x84

    int CanSweep(void) const;
    void GetSupport(const mVec3 &dir, const mOCS &ocs, mVec3 *out) const;
};

#endif
