// eCollision::GetEdgeFaceContacts @ 0x0002b4dc  (eAll_psp.obj)
// static int (const eCollisionSupport*, const eCollisionSupport*, eContactResult*)
// Split-TU: GetEdgeFaceContacts is NOT declared in include/eCollision.h, so the
// class is declared locally here (along with the support/result structs).
// Twin of eCollision::GetEdgeTriContacts (0x2b43c); the only semantic difference
// is the 4th argument to Clip (-1 here vs the feature field in the Tri variant).

typedef int v4sf_t __attribute__((mode(V4SF)));

struct eCollisionSupport {
    v4sf_t verts[64];     // 0 .. 0x3ff
    int    index;         // 0x400
    char   pad[1352 - 1028];
    int    feature;       // 0x548
    char   pad2[0x554 - 0x54c];
    int    edge554;       // 0x554
};

struct eContactResult {
    v4sf_t pa;            // 0x00
    v4sf_t pb;            // 0x10
    unsigned short fa0 : 4;
    unsigned short fa1 : 6;
    unsigned short fa2 : 6;
    unsigned short fb0 : 4;
    unsigned short fb1 : 6;
    unsigned short fb2 : 6;
    char c0;
    char c1;
    char c2;
    char c3;
};

class eCollision {
public:
    static int Clip(const eCollisionSupport *, const eCollisionSupport *, eContactResult *, unsigned int);
    static int GetEdgeFaceContacts(const eCollisionSupport *, const eCollisionSupport *, eContactResult *);
};

int eCollision::GetEdgeFaceContacts(const eCollisionSupport *a, const eCollisionSupport *b, eContactResult *r) {
    if (a->edge554 == 0) {
        return 0;
    }
    int n = eCollision::Clip(b, a, r, (unsigned int)-1);
    int i = 0;
    if (i < n) {
        v4sf_t t[1];
        v4sf_t *pa = &r[i].pa;
        v4sf_t *pb = &r->pb;
        do {
            t[0] = *pa;
            *pa = *pb;
            i++;
            *pb = t[0];
            pa = (v4sf_t *)((char *)pa + 48);
            pb = (v4sf_t *)((char *)pb + 48);
        } while (i < n);
    }
    return n;
}
