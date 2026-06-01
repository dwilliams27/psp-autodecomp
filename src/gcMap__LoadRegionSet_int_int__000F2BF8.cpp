// gcMap::LoadRegionSet(int, int)  @ 0x000f2bf8
// Symbol: __0fFgcMapNLoadRegionSetiTB
// Obj: gcAll_psp.obj
//
// Split-TU isolated build: class declared locally to avoid touching the shared
// header. Two index-bounded lookups into SN-style cBaseArray (count at [-4]),
// then a forwarding call into gcMap::LoadRegionSet(const cGUIDT<gcRegion> *).

template <class T> class cGUIDT;
class gcRegion;

class gcMap {
public:
    char pad[0x398];
    void **mGroups;   // 0x398 : cBaseArray of group ptrs

    void LoadRegionSet(int groupIdx, int setIdx);
    void LoadRegionSet(const cGUIDT<gcRegion> *guids);
};

void gcMap::LoadRegionSet(int groupIdx, int setIdx) {
    void *group;
    if (groupIdx >= 0) {
        group = mGroups;
        int count = 0;
        if (group != 0) {
            count = ((int *)group)[-1];
        }
        if (groupIdx < count) {
            goto valid_group;
        }
    }
    group = 0;
    goto have_group;
valid_group:
    group = ((void **)group)[groupIdx];
have_group:

    void *set = 0;
    if (group != 0) {
        if (setIdx >= 0) {
            group = *(void **)((char *)group + 0x20);
            int count = 0;
            if (group != 0) {
                count = ((int *)group)[-1];
            }
            if (setIdx < count) {
                goto valid_set;
            }
        }
        set = 0;
        goto have_set;
    valid_set:
        set = ((void **)group)[setIdx];
    have_set:;
    }

    if (set != 0) {
        LoadRegionSet((const cGUIDT<gcRegion> *)((char *)set + 8));
    }
}
