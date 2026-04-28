#ifndef EMESHSHAPE_H
#define EMESHSHAPE_H

class eCollisionContactInfo;
class eHeightmapShape;
class cBase;
class cMemPool;
class mOCS;

class eMeshShape {
public:
    eMeshShape(cBase *);

    int Collide(const eMeshShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eHeightmapShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    float GetVolume(void) const;

    static eMeshShape *New(cMemPool *, cBase *);
};

#endif
