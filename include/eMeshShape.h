#ifndef EMESHSHAPE_H
#define EMESHSHAPE_H

class eCollisionContactInfo;
class eHeightmapShape;
class mOCS;

class eMeshShape {
public:
    int Collide(const eMeshShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eHeightmapShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    float GetVolume(void) const;
};

#endif
