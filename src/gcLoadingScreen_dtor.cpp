class cMemPool;

class cFactory {
public:
    void DeleteGroups(void);
};

void cObject___dtor_cObject_void(void *, int);
void *cMemPool_GetPoolFromPtr(void *);

extern char gcLoadingScreenvirtualtable[];
extern char cObjectvirtualtable_for_gcLoadingScreen[];
extern int gcLoadingScreen_singleton;

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

class gcLoadingScreen : public cFactory {
public:
};

extern "C" {

void gcLoadingScreen___dtor_gcLoadingScreen_void(gcLoadingScreen *self, int flags) {
    if (self != 0) {
        *(void **)((char *)self + 4) = gcLoadingScreenvirtualtable;
        self->cFactory::DeleteGroups();
        gcLoadingScreen_singleton = 0;
        *(void **)((char *)self + 4) = cObjectvirtualtable_for_gcLoadingScreen;
        cObject___dtor_cObject_void(self, 0);
        if (flags & 1) {
            void *pool = cMemPool_GetPoolFromPtr(self);
            void *block = *(void **)((char *)pool + 0x24);
            DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
            short off = rec->offset;
            rec->fn((char *)block + off, self);
        }
    }
}

}
