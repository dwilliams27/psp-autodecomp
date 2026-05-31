// gcRegionSetGroup::ClearRegionSetState(int) const  @ 0x000efc00
// Split-TU isolated build. Class declared locally to avoid touching shared header.

template <class T> class cGUIDT;
class gcRegion;
class gcMap;

class gcMap {
public:
    void ClearRegionState(const cGUIDT<gcRegion> &) const;
};

extern gcMap *D_0037D7FC;

class gcRegionSetGroup {
public:
    char pad[0x20];
    void *m_arrayData;            // 0x20 (cBaseArray data ptr)
    void *m_owner;                // 0x24

    void ClearRegionSetState(int) const;
};

void gcRegionSetGroup::ClearRegionSetState(int idx) const {
    gcMap *map = D_0037D7FC;
    if (map == 0) {
        return;
    }
    void **items = (void **)m_arrayData;
    if (idx < 0) {
        int i = 0;
        int offset = 0;
        do {
            int count = 0;
            if (m_arrayData != 0) {
                count = ((int *)m_arrayData)[-1];
            }
            if (i >= count) {
                break;
            }
            char *regionSet = *(char **)((char *)m_arrayData + offset);
            if (regionSet != 0) {
                int k = 0;
                char *p = regionSet + 8;
                do {
                    map->ClearRegionState(*(const cGUIDT<gcRegion> *)p);
                    k++;
                    p += 8;
                } while (k < 2);
            }
            i++;
            offset += 4;
        } while (1);
    } else {
        int count = 0;
        if (items != 0) {
            count = ((int *)items)[-1];
        }
        if (idx < count) {
            char *regionSet = *(char **)((char *)items + (idx * 4));
            if (regionSet != 0) {
                int k = 0;
                char *p = regionSet + 8;
                do {
                    map->ClearRegionState(*(const cGUIDT<gcRegion> *)p);
                    k++;
                    p += 8;
                } while (k < 2);
            }
        }
    }
}
