// 0x000646d0, 68B, leaf: virtual interface dispatch to shape's collision method.
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

class eSweptSphereCollisionHandler : public eSweptVolumeCollisionHandler {
public:
    void ProcessCollision(const eCollisionInfo &info);
};

#pragma control sched=1
void eSweptSphereCollisionHandler::ProcessCollision(const eCollisionInfo &info) {
    eCollisionObj *obj = (eCollisionObj *)info.obj;
    char *vt = (char *)obj->vt;
    short *slot = (short *)(vt + 0x88);
    void *adjusted = (char *)obj + *slot;
    __asm__ volatile("" ::: "memory");
    typedef void (*Method)(void *, const eCollisionInfo &, int, void *, void *, void *);
    void *p1 = (char *)this + 0x4960;
    void *p2 = (char *)this + 0x80;
    void *p3 = (char *)this + 0xD0;
    Method m = *(Method *)((char *)slot + 4);
    m(adjusted, info, 1, p1, p2, p3);
}
