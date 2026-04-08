#ifndef EHEIGHTMAPSHAPE_H
#define EHEIGHTMAPSHAPE_H

class eBoxShape;
class eCollisionContactInfo;
class eMeshShape;
class eSphereShape;
class mBox;
class mOCS;
class mVec3;

class eHeightmapShape {
public:
    int Collide(const eBoxShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eSphereShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eMeshShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eHeightmapShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    void GetAABB(mBox *, const mOCS &) const;
    void GetInertialTensor(float, mVec3 *) const;
    int GetTileIndex(void) const;
    int GetTileTriCount(void) const;
    float GetVolume(void) const;
};

#endif
