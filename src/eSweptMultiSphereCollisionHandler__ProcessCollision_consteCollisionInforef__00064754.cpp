// 0x00064754, 72B, leaf: virtual interface dispatch to shape's collision method.
// Split-TU local redeclaration of the targeted method only.

struct eCollisionInfo {
    void *obj;
};

struct eCollisionObj {
    int unk0;
    void *vt;
};

class eSweptVolumeCollisionHandler {
public:
};

class eSweptMultiSphereCollisionHandler : public eSweptVolumeCollisionHandler {
public:
    void ProcessCollision(const eCollisionInfo &info);
};

#pragma control sched=1
void eSweptMultiSphereCollisionHandler::ProcessCollision(const eCollisionInfo &info) {
    eCollisionObj *obj = (eCollisionObj *)info.obj;
    char *vt = (char *)obj->vt;
    short *slot = (short *)(vt + 0x88);
    void *adjusted = (char *)obj + *slot;
    __asm__ volatile("" ::: "memory");
    typedef void (*Method)(void *, const eCollisionInfo &, int, void *, void *, void *);
    int v = *(int *)((char *)this + 0x4950);
    void *p1 = (char *)this + 0x49D0;
    void *p2 = (char *)this + 0x4A10;
    void *p3 = (char *)this + 0xD0;
    Method m = *(Method *)((char *)slot + 4);
    m(adjusted, info, v, p1, p2, p3);
}
