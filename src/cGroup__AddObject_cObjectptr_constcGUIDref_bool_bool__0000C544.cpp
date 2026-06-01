// cGroup::AddObject(cObject *, const cGUID &, bool, bool)
// Address: 0x0000c544, Size: 180B
// Obj: cAll_psp.obj
// Symbol: __0fGcGroupJAddObjectP6HcObjectRC6FcGUIDbTD
//
// Split-TU: cGroup / cObject / cGUID / cManager declared locally here.

struct cGUID {
    int mB;
    int mA;
};

struct cObject {
    void *group;          // 0x0
    char _pad0[0x1C];
    cGUID guid;           // 0x20
    unsigned short flags; // 0x28
    char _pad1[0xA];
    cObject *prev;        // 0x34
    cObject *next;        // 0x38
};

class cManager {
public:
    static void Add(cObject *);
};

class cGroup {
public:
    char _pad[0x8];
    unsigned char mFlag;  // 0x8
    char _pad2[0x3];
    cObject *mList;   // 0xC

    void AddObject(cObject *obj, const cGUID &guid, bool b1, bool b2);
};

void cGroup::AddObject(cObject *obj, const cGUID &guid, bool b1, bool b2) {
    int extra = 0;
    __asm__ volatile("" ::: "memory");
    cObject *head = mList;
    if (b2 || mFlag != 0) {
        extra = 1;
    }
    unsigned char extraMasked = (unsigned char)extra;

    if (head != 0) {
        obj->next = head->next;
        obj->prev = mList;
        obj->next->prev = obj;
        obj->prev->next = obj;
    } else {
        mList = obj;
        obj->prev = obj;
        obj->next = obj;
    }

    obj->guid = guid;
    obj->group = this;

    if (b1) {
        obj->flags = (unsigned short)(obj->flags | 2);
    }
    if (extraMasked) {
        obj->flags = (unsigned short)(obj->flags | 4);
    }

    cManager::Add(obj);
}
