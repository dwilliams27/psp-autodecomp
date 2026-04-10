struct eDynamicMeshMorphVert;

class eDynamicMeshMorphTarget {
public:
    int mCount;
    float mWeight;
    eDynamicMeshMorphVert *mVerts;

    eDynamicMeshMorphTarget();
};

eDynamicMeshMorphTarget::eDynamicMeshMorphTarget() {
    mCount = 0;
    __asm__ volatile("" ::: "memory");
    mWeight = 0.0f;
    mVerts = 0;
}
