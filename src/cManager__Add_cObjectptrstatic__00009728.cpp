// cManager::Add(cObject *) static
// Address: 0x00009728, Size: 252B
// Obj: cAll_psp.obj
// Mangled: __0fIcManagerDAddP6HcObjectT

struct cObject {
    char _pad00[0x20];
    int   key0;        // 0x20
    int   key1;        // 0x24
    char _pad28[0x30 - 0x28];
    int   id;          // 0x30
    char _pad34[0x3C - 0x34];
    cObject *prev;     // 0x3C
    cObject *next;     // 0x40
};

class cManager {
public:
    static int Add(cObject *);
};

extern int        *cManager__GetLock();
extern int         cManager__FindEmptyIndex();
extern int         cIRand(void);
extern "C" int     sceKernelLockMutex(int, int, int);
extern "C" int     sceKernelUnlockMutex(int, int);

extern cObject    *g_obj_table[];   // 0x38890
extern cObject    *g_obj_hash[];    // 0x40890

int cManager::Add(cObject *obj) {
    int idx;

    sceKernelLockMutex(*cManager__GetLock(), 1, 0);
    idx = cManager__FindEmptyIndex();
    if (idx >= 0) {
        obj->id = (((cIRand() | 1) & 0xFFFF) << 16) | idx;
        g_obj_table[idx] = obj;
        int h = 0;
        cObject **base = g_obj_hash;
        if (obj != 0) {
            h = obj->key0 ^ obj->key1;
        }
        cObject **bucket = (cObject **)(((h & 0xFF) << 2) + (int)base);
        cObject *head = *bucket;
        if (head != 0) {
            obj->next = head->next;
            obj->prev = *bucket;
            obj->next->prev = obj;
            obj->prev->next = obj;
        } else {
            *bucket = obj;
            obj->prev = obj;
            obj->next = obj;
        }
    }
    sceKernelUnlockMutex(*cManager__GetLock(), 1);
    return idx >= 0;
}
