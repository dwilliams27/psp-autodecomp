#ifndef EDYNAMICMODEL_H
#define EDYNAMICMODEL_H

class cMemPool;

class eDynamicModel {
public:
    void PlatformReset(cMemPool *, bool);
    void OnRemovedFromWorld(void);
};

#endif
