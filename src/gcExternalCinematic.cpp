// gcExternalCinematic::gcExternalCinematic(cBase *) @ 0x000eae18
// cObject base, gcCinematic embedded member at +0x44. The vtable assignment
// for gcExternalCinematic is emitted between the cObject ctor and the
// gcCinematic member ctor — easiest to express by calling the member ctor
// manually inside the body.

class cBase;

class cObject {
public:
    cObject(cBase *);
    char _cObjectPad[0x44];
};

class gcExternalCinematic : public cObject {
public:
    gcExternalCinematic(cBase *);
};

extern char gcExternalCinematicvirtualtable[];
extern void gcCinematic_gcCinematic(void *self, cBase *parent);

gcExternalCinematic::gcExternalCinematic(cBase *parent) : cObject(parent) {
    *(void **)((char *)this + 4) = gcExternalCinematicvirtualtable;
    gcCinematic_gcCinematic((char *)this + 0x44, (cBase *)this);
}
