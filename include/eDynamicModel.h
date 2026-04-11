#ifndef EDYNAMICMODEL_H
#define EDYNAMICMODEL_H

class cMemPool;
class eDynamicMesh;

class eDynamicModel {
public:
    void PlatformReset(cMemPool *, bool);
    void OnRemovedFromWorld(void);
    bool NeedsSkinning(const eDynamicMesh *, int, int *) const;
};

#endif
