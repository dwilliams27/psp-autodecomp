// eConvexHullUtil::AddPolygon(const mVec3*, const unsigned char*, int)
// Address: 0x0006e6e4, Size: 152B, Obj: eAll_psp.obj
//
// Triangle-fan: AddTriangle(verts[idx[0]], verts[idx[i+1]], verts[idx[i+2]]).
// Loading v0 (the fan-origin vertex) before the memory barrier lets SNC preload
// its lbu into the bnezl branch-likely delay slot (snc-branch-likely.md Pattern 4),
// while the barrier forces left-to-right sequential arg evaluation in the body.

#pragma control sched=1
struct mVec3 { unsigned int qw __attribute__((mode(TI))); };
class eConvexHullUtil {
public:
    void AddTriangle(const mVec3 &, const mVec3 &, const mVec3 &);
    void AddPolygon(const mVec3 *verts, const unsigned char *indices, int count);
};
void eConvexHullUtil::AddPolygon(const mVec3 *verts, const unsigned char *indices, int count) {
    int i = 0;
    int last = count - 2;
    while (i < last) {
        const mVec3 &v0 = verts[indices[0]];
        __asm__ volatile("" ::: "memory");
        AddTriangle(v0, verts[(indices + i)[1]], verts[(indices + i)[2]]);
        i += 1;
    }
}
