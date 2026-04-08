#ifndef EMESHSHAPE_H
#define EMESHSHAPE_H

class eCollisionContactInfo;
class mOCS;

class eMeshShape {
public:
    int Collide(const eMeshShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
};

#endif
