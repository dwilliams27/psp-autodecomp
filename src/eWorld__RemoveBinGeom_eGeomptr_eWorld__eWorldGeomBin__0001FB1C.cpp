// eWorld::RemoveBinGeom(eGeom *, eWorld::eWorldGeomBin)
// Address: 0x0001fb1c, Size: 104B  Obj: eAll_psp.obj
// Removes a geom from a per-bin doubly-linked list, fixing the bin head.

#pragma control sched=1

class eGeom {
public:
    char pad[0x6c];
    eGeom *next; // 0x6c
    eGeom *prev; // 0x70
};

class eWorld {
public:
    int field_0x00;
    eGeom *bins[1]; // 0x04 indexed array of bin heads

    enum eWorldGeomBin {};

    void RemoveBinGeom(eGeom *geom, eWorldGeomBin bin);
};

void eWorld::RemoveBinGeom(eGeom *geom, eWorldGeomBin bin)
{
    if (geom != 0 && geom->next != 0 && geom->prev != 0) {
        if (bins[bin] == geom)
            bins[bin] = geom->prev;
        geom->next->prev = geom->prev;
        geom->prev->next = geom->next;
        geom->next = 0;
        geom->prev = 0;
        if (bins[bin] == geom)
            bins[bin] = 0;
    }
}
