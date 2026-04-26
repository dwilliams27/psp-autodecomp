// eGeomTrail::eGeomTrail(cBase *) @ 0x00079460
// Inherits eDynamicGeom : eGeom : cObject. In sched=1 zone.
#pragma control sched=1

class cBase;

class eGeom {
public:
    eGeom(cBase *);
    char _eGeomPad[0x90];
};

class eDynamicGeom : public eGeom {
public:
    eDynamicGeom(cBase *);
    char _dynPad[0x58];
};

class eGeomTrail : public eDynamicGeom {
public:
    eGeomTrail(cBase *);
    char _trailPad[0x30];
};

extern char eGeomTrailvirtualtable[];

eGeomTrail::eGeomTrail(cBase *base) : eDynamicGeom(base) {
    *(void **)((char *)this + 4) = eGeomTrailvirtualtable;
    *(int *)((char *)this + 0xF0) = 0;
    *(int *)((char *)this + 0xF8) = 0;
    *(int *)((char *)this + 0xFC) = 0;
}
