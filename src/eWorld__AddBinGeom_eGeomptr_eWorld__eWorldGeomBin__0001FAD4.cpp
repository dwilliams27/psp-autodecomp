// eWorld::AddBinGeom(eGeom *, eWorld::eWorldGeomBin)
// Address: 0x0001fad4, Size: 72B  Obj: eAll_psp.obj
// Inserts a geom into a per-bin circular doubly-linked list.

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

    void AddBinGeom(eGeom *geom, eWorldGeomBin bin);
};

void eWorld::AddBinGeom(eGeom *geom, eWorldGeomBin bin)
{
    eGeom *head = bins[bin];
    if (head != 0) {
        geom->next = head->next;
        geom->prev = bins[bin];
        geom->next->prev = geom;
        geom->prev->next = geom;
        return;
    }
    bins[bin] = geom;
    geom->prev = geom;
    geom->next = geom;
    __asm__ volatile("" ::: "memory");
}
