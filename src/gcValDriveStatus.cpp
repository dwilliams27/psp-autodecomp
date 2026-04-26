// -----------------------------------------------------------------------------
// gcValDriveStatus::AssignCopy(const cBase *)  @ 0x00325b10
//   (gcAll_psp.obj, 48B)
// -----------------------------------------------------------------------------

class cBase;

class gcValDriveStatus {
public:
    int pad0;
    int pad4;
    int f8;
    void AssignCopy(const cBase *);
};

gcValDriveStatus *dcast(const cBase *);

void gcValDriveStatus::AssignCopy(const cBase *base) {
    gcValDriveStatus *other = dcast(base);
    this->f8 = other->f8;
}
