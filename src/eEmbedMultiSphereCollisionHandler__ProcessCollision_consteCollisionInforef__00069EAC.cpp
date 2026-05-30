// 0x00069eac, 68B, leaf: virtual interface dispatch to shape's collision method.
// Split-TU local redeclaration of the targeted method only.

struct eCollisionInfo {
    void *obj;
};

struct eCollisionObj {
    int unk0;
    void *vt;
};

class eEmbedVolumeCollisionHandler {
public:
};

class eEmbedMultiSphereCollisionHandler : public eEmbedVolumeCollisionHandler {
public:
    void ProcessCollision(const eCollisionInfo &info);
};

#pragma control sched=1
void eEmbedMultiSphereCollisionHandler::ProcessCollision(const eCollisionInfo &info) {
    eCollisionObj *obj = (eCollisionObj *)info.obj;
    char *vt = (char *)obj->vt;
    short *slot = (short *)(vt + 0x90);
    void *adjusted = (char *)obj + *slot;
    __asm__ volatile("" ::: "memory");
    typedef void (*Method)(void *, const eCollisionInfo &, int, void *, void *);
    int v = *(int *)((char *)this + 0x48c0);
    void *p1 = (char *)this + 0x4940;
    void *p2 = (char *)this + 0x40;
    Method m = *(Method *)((char *)slot + 4);
    m(adjusted, info, v, p1, p2);
}
