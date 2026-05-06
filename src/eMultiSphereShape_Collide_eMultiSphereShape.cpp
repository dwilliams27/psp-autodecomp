// ODR-WARNING: this TU intentionally redeclares eMultiSphereShape with a
// minimal member list for the Collide(const eMultiSphereShape*) overload.
// Do not include eMultiSphereShape.h here; adding this method to the shared
// header can perturb already-matched siblings in eMultiSphereShape.cpp.

struct mOCS;
class eCollisionContactInfo;

class eMultiSphereShape {
public:
    int Collide(const eMultiSphereShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
};

class eCollision {
public:
    static int MultiSphereMultiSphere(const eMultiSphereShape &, const eMultiSphereShape &, const mOCS &, const mOCS &, eCollisionContactInfo *);
};

#pragma control sched=1
int eMultiSphereShape::Collide(const eMultiSphereShape *shape, int, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    eCollisionContactInfo *contact = info;
    __asm__ volatile("" : "+r"(contact));
    int hit = eCollision::MultiSphereMultiSphere(*shape, *this, ocs2, ocs1, contact);
    int i = 0;
    if (hit != 0) {
        int count = *(int *)((char *)contact + 0x14);
        if (i < count) {
            char *p = (char *)contact + 0x20;
            do {
                __asm__ volatile(
                    "lv.q C120, 0(%0)\n"
                    "vneg.t C120, C120\n"
                    "sv.q C120, 0(%0)\n"
                    :: "r"(p) : "memory"
                );
                i++;
                p += 0x40;
                count = *(int *)((char *)contact + 0x14);
            } while (i < count);
        }
        return 1;
    }
    return 0;
}
