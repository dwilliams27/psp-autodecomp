#ifndef ENAVMESH_H
#define ENAVMESH_H

#include "cObject.h"

class cMemPool;
class cBase;
class cType;
struct mVec3;
struct eNavMeshPos;

class eNavMesh : public cObject {
public:
    eNavMesh(cBase *);
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
    static eNavMesh *FindNavMeshFromPoint(const mVec3 *, unsigned char &);
    static int FindNavMeshFromPoint(eNavMeshPos *);
    void AssignCopy(const cBase *);
    void Reset(cMemPool *, bool);
};

#endif
