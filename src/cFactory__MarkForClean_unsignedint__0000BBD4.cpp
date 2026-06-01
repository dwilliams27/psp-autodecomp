// cFactory::MarkForClean(unsigned int)
// Address: 0x0000bbd4, Obj: cAll_psp.obj
// Symbol: __0fIcFactoryMMarkForCleanUi
// ODR-WARNING: cFactory declared locally (split-TU); do not include canonical header.

class cFactory {
public:
    void MarkForClean(unsigned int);
};

struct VisitDispatchEntry {
    short offset;
    short _pad;
    void (*fn)(void *, unsigned int, void *, void *, int, int);
};

struct ChildArrayDispatchEntry {
    short offset;
    short _pad;
    void **(*fn)(void *, int *);
};

void cFactory::MarkForClean(unsigned int mask) {
    char *classdesc = *(char **)((char *)this + 4);

    VisitDispatchEntry *ve = (VisitDispatchEntry *)(classdesc + 0x60);
    ve->fn((char *)this + ve->offset, mask, 0, 0, 0, 1);

    classdesc = *(char **)((char *)this + 4);
    int count = 0;
    ChildArrayDispatchEntry *ce = (ChildArrayDispatchEntry *)(classdesc + 0xB0);
    void **children = ce->fn((char *)this + ce->offset, &count);
    for (int i = 0; i < count; i++) {
        void *c = children[i];
        if (c != 0) {
            ((cFactory *)c)->MarkForClean(mask);
        }
    }
}
