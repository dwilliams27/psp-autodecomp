// eHeightmap::GetEmbedContacts(const eCollisionInfo &, int, const mSphere *,
//                              eContactCollector *) const
// Address: 0x00052c78, Size: 64B, Obj: eAll_psp.obj
//
// Forwards to the shape's virtual GetEmbedContacts (vtable entry at 0xB0),
// reordering args and passing the eCollisionInfo through as a trailing arg.
// Structurally identical to the matched eStaticModel / eDynamicModel siblings.

class eCollisionInfo;
class eContactCollector;
class mSphere;

class eHeightmap {
public:
    void GetEmbedContacts(const eCollisionInfo &, int, const mSphere *,
                          eContactCollector *) const;
};

#pragma control sched=1

void eHeightmap::GetEmbedContacts(const eCollisionInfo &info, int idx,
                                  const mSphere *sphere,
                                  eContactCollector *collector) const {
    char *shape = ((char **)&info)[1];
    int *entry = (int *)(((char **)shape)[1] + 0xB0);
    char *adjusted = shape + *(short *)entry;
    __asm__ volatile("" ::: "memory");
    ((void (*)(char *, int, const mSphere *, const eCollisionInfo &,
               eContactCollector *))entry[1])
        (adjusted, idx, sphere, info, collector);
}
