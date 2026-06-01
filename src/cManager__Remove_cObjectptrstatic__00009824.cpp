// cManager::Remove(cObject *) static
// Address: 0x00009824, Size: 224B
// Obj: cAll_psp.obj
// Mangled: __0fIcManagerGRemoveP6HcObjectT

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
    static void Remove(cObject *);
};

extern int        *cManager__GetLock();
extern "C" int     sceKernelLockMutex(int, int, int);
extern "C" int     sceKernelUnlockMutex(int, int);

extern cObject    *g_obj_table[];   // 0x38890
extern cObject    *g_obj_hash[];    // 0x40890

#pragma control sched=2
void cManager::Remove(cObject *obj) {
    sceKernelLockMutex(*cManager__GetLock(), 1, 0);

    g_obj_table[obj->id & 0xFFFF] = 0;

    int h = obj ? (obj->key0 ^ obj->key1) : 0;
    h &= 0xFF;

    if (obj != 0) {
        if (obj->next != 0) {
            if (obj->prev != 0) {
                cObject **bucket = (cObject **)((h << 2) + (int)g_obj_hash);
                if (*bucket == obj) {
                    *bucket = obj->prev;
                }
                obj->next->prev = obj->prev;
                obj->prev->next = obj->next;
                obj->next = 0;
                obj->prev = 0;
                if (*bucket == obj) {
                    *bucket = 0;
                }
            }
        }
    }

    sceKernelUnlockMutex(*cManager__GetLock(), 1);
}
