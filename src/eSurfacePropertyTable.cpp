// eSurfacePropertyTable - eAll_psp.obj
//   0x001f6f28 AssignCopy(const cBase *)

#include "cBase.h"
#include "cObject.h"

template <class T> T *dcast(const cBase *);

class cBaseArray {
public:
    cBaseArray &operator=(const cBaseArray &);
};

template <class T>
class cArrayBase {
public:
    cArrayBase &operator=(const cArrayBase &);
};

class eSurfacePropertyPair;

class eSurfacePropertyTable : public cObject {
public:
    void AssignCopy(const cBase *);
};

#pragma control sched=1
void eSurfacePropertyTable::AssignCopy(const cBase *base) {
    eSurfacePropertyTable *other = dcast<eSurfacePropertyTable>(base);
    cObject::operator=(*other);
    ((cBaseArray *)((char *)this + 0x44))->operator=(
        *(const cBaseArray *)((char *)other + 0x44));
    ((cArrayBase<eSurfacePropertyPair> *)((char *)this + 0x4C))->operator=(
        *(const cArrayBase<eSurfacePropertyPair> *)((char *)other + 0x4C));
}
#pragma control sched=2
