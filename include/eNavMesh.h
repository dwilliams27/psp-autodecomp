#ifndef ENAVMESH_H
#define ENAVMESH_H

class cMemPool;
class cBase;
struct mVec3;
struct eNavMeshPos;

class eNavMesh {
public:
    static cBase *New(cMemPool *, cBase *);
    static eNavMesh *FindNavMeshFromPoint(const mVec3 *, unsigned char &);
    static int FindNavMeshFromPoint(eNavMeshPos *);
    void Reset(cMemPool *, bool);
};

#endif
