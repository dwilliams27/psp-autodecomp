// cFactory::DeleteMarkedForClean(unsigned int, bool)
// Address: 0x0000bc90, Obj: cAll_psp.obj
// Symbol: cFactory__DeleteMarkedForClean_unsignedint_bool__0000BC90
// ODR-WARNING: cFactory declared locally (split-TU); do not include canonical header.

class cFactory {
public:
    void DeleteMarkedForClean(unsigned int, bool);
    void CleanGroups(unsigned int, bool);
};

struct GroupArrayDispatchEntry {
    short offset;
    short _pad;
    void **(*fn)(void *, int *);
};

void cFactory::DeleteMarkedForClean(unsigned int flags, bool b) {
    char *classdesc = *(char **)((char *)this + 4);
    int count = 0;
    GroupArrayDispatchEntry *entry = (GroupArrayDispatchEntry *)(classdesc + 176);
    void **groups = entry->fn((char *)this + entry->offset, &count);
    for (int i = 0; i < count; i++) {
        void *obj = groups[i];
        if (obj != 0) {
            ((cFactory *)obj)->DeleteMarkedForClean(flags, false);
        }
    }
    CleanGroups(0, b);
}
