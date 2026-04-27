// eConvexHullShape::GetInertialTensor(float, mVec3 *) const @ 0x00071640
// Trivial wrapper around GetInertialTensorAndVolume that conditionally
// caches the volume back into this+0xA0 if it's currently zero.

class mVec3;

class eConvexHullShape {
public:
    void GetInertialTensor(float, mVec3 *) const;
    void GetInertialTensorAndVolume(float, mVec3 *, float *) const;
};

#pragma control sched=1
void eConvexHullShape::GetInertialTensor(float mass, mVec3 *out) const {
    float vol;
    GetInertialTensorAndVolume(mass, out, &vol);
    if (*(float *)((char *)this + 0xA0) == 0.0f) {
        *(float *)((char *)this + 0xA0) = vol;
    }
}
#pragma control sched=2
