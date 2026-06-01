// gcDesiredEntityTemplate::GetRelationships(cHandleT<gcEnumeration>, int, cObject **) const
// 0x001284d8, 108B, gcAll_psp.obj
//
// Forwards to the underlying entity template:
//   gcEntityTemplate* t = Get(true);
//   if (t) return t->GetRelationships(handle, n, objs);
//   return 0;
//
// Get(bool) is called first (clobbers a1=handle), so the handle is spilled to
// the stack and reloaded for the forwarded call.

class cObject;
class gcEnumeration;

template <class T>
class cHandleT {
public:
    int mIndex;
};

class gcEntityTemplate {
public:
    int GetRelationships(cHandleT<gcEnumeration>, int, cObject **) const;
};

class gcDesiredEntityTemplate {
public:
    cObject *Get(bool) const;
    int GetRelationships(cHandleT<gcEnumeration>, int, cObject **) const;
};

int gcDesiredEntityTemplate::GetRelationships(cHandleT<gcEnumeration> handle,
                                              int n, cObject **objs) const {
    gcEntityTemplate *t = (gcEntityTemplate *)Get(true);
    if (t) {
        return t->GetRelationships(handle, n, objs);
    }
    return 0;
}
