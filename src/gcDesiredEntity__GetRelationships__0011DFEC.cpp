// gcDesiredEntity::GetRelationships(cHandleT<gcEnumeration>, int, cObject **) const
// Address: 0x0011dfec, Size: 108B
// Obj: gcAll_psp.obj
//
// Structure (from disasm):
//   gcEntity *e = this->Get(true);
//   if (e) return e->GetRelationships(handle, n, objs);
//   return 0;

class gcEnumeration;
class cObject;

template <class T>
class cHandleT {
public:
    int mIndex;
};

class gcEntity {
public:
    int GetRelationships(cHandleT<gcEnumeration>, int, cObject **) const;
};

class gcDesiredEntity {
public:
    gcEntity *Get(bool) const;
    int GetRelationships(cHandleT<gcEnumeration>, int, cObject **) const;
};

int gcDesiredEntity::GetRelationships(cHandleT<gcEnumeration> handle, int n, cObject **objs) const {
    gcEntity *e = Get(true);
    if (e) {
        return e->GetRelationships(handle, n, objs);
    }
    return 0;
}
