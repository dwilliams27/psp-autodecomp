// eRoom::ClearRoomGeomList(eGeom *) @ 0x0003f294
// symbol: __0fFeRoomRClearRoomGeomListP6FeGeom

struct GeomLink {
    GeomLink *m0;       // +0x00
    void     *m4;       // +0x04  owner (eGeom*)
    GeomLink *m8;       // +0x08  list A prev
    GeomLink *mC;       // +0x0C  list A next
    GeomLink *m10;      // +0x10  list B prev
    GeomLink *m14;      // +0x14  list B next
};

struct GeomOwner {
    char     pad[0x110];
    GeomLink *m110;     // +0x110  list A head
};

struct FreeMgr {
    void     *m0;
    GeomLink *m4;       // +0x04  free list head
};

struct eGeom {
    char     pad[0x7C];
    GeomLink *m7C;      // +0x7C  geom link list head
};

class eRoom {
public:
    void ClearRoomGeomList(eGeom *geom);
};

extern FreeMgr *gFreeMgr;

void eRoom::ClearRoomGeomList(eGeom *geom)
{
    GeomLink *node = geom->m7C;
    while (node != 0) {
        GeomOwner *owner = (GeomOwner *)node->m4;

        if (node->m10 != 0) {
            GeomLink *bn = node->m14;
            if (bn != 0) {
                geom->m7C = bn;
                node->m10->m14 = node->m14;
                node->m14->m10 = node->m10;
                node->m10 = 0;
                node->m14 = 0;
                if (geom->m7C == node) {
                    geom->m7C = 0;
                }
            }
        }

        if (node->m8 != 0) {
            if (node->mC != 0) {
                if (owner->m110 == node) {
                    owner->m110 = node->mC;
                }
                node->m8->mC = node->mC;
                node->mC->m8 = node->m8;
                node->m8 = 0;
                node->mC = 0;
                if (owner->m110 == node) {
                    owner->m110 = 0;
                }
            }
        }

        node->m8 = 0;
        node->mC = 0;
        node->m10 = 0;
        node->m14 = 0;
        node->m0 = 0;
        node->m4 = 0;

        FreeMgr *mgr = gFreeMgr;
        GeomLink *head = mgr->m4;
        if (head != 0) {
            node->m8 = head->m8;
            node->mC = gFreeMgr->m4;
            node->m8->mC = node;
            node->mC->m8 = node;
        } else {
            mgr->m4 = node;
            node->mC = node;
            node->m8 = node;
        }

        node = geom->m7C;
    }
}
