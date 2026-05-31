// eNavMesh::GetSolidVerts(unsigned char, eNavMesh::eNavEdge, int *) const
// Address: 0x0005a140, Size: 260B, Obj: eAll_psp.obj

struct eNavTri {
    unsigned char v[3];   // 0x0, 0x1, 0x2
    char pad3[3];
    unsigned char flags;  // 0x6
    char pad7[1];
};

class eNavMesh {
public:
    enum eNavEdge { kEdgeNone = 0 };
    int GetSolidVerts(unsigned char tri, eNavEdge edge, int *out) const;
};

int eNavMesh::GetSolidVerts(unsigned char tri, eNavEdge edge, int *out) const {
    eNavTri *node = *(eNavTri **)((const char *)this + 0x44) + tri;
    __asm__ volatile("" ::: "memory");
    out[0] = -1;
    out[1] = -1;
    if (node != 0) {
        if (edge == 1) {
            if (node->flags & 1) {
                out[0] = node->v[0];
            }
            if (node->flags & 2) {
                out[1] = node->v[1];
            }
            return (node->flags & 3) > 0;
        }
        if (edge == 2) {
            if (node->flags & 2) {
                out[0] = node->v[1];
            }
            if (node->flags & 4) {
                out[1] = node->v[2];
            }
            return (node->flags & 6) > 0;
        }
        if (edge == 3) {
            if (node->flags & 4) {
                out[0] = node->v[2];
            }
            if (node->flags & 1) {
                out[1] = node->v[0];
            }
            return (node->flags & 5) > 0;
        }
    }
    return 0;
}
