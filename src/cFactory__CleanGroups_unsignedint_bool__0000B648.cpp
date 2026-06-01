// cFactory::CleanGroups(unsigned int, bool)
// Address: 0x0000b648, Obj: cAll_psp.obj
// Symbol: cFactory__CleanGroups_unsignedint_bool__0000B648
// ODR-WARNING: cFactory declared locally (split-TU); do not include canonical header.

class cGroup {
public:
    bool Clean(unsigned int, bool);
};

class cFactory {
public:
    bool CleanGroups(unsigned int, bool);
};

struct GroupArrayDispatchEntry {
    short offset;
    short _pad;
    cGroup **(*fn)(void *, int *);
};

bool cFactory::CleanGroups(unsigned int flags, bool b) {
    char *classdesc = *(char **)((char *)this + 4);
    int count = 0;
    GroupArrayDispatchEntry *entry = (GroupArrayDispatchEntry *)(classdesc + 168);
    cGroup **groups = entry->fn((char *)this + entry->offset, &count);
    for (int i = 0; i < count; i++) {
        cGroup *obj = groups[i];
        if (obj != 0 && !obj->Clean(flags, b)) {
            return false;
        }
    }
    return true;
}
