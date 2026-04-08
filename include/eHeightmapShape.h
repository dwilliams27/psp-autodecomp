#ifndef EHEIGHTMAPSHAPE_H
#define EHEIGHTMAPSHAPE_H

class eCollisionContactInfo;
class eMeshShape;
class mOCS;

class eHeightmapShape {
public:
    int Collide(const eMeshShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eHeightmapShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int GetTileTriCount(void) const;
};

#endif
