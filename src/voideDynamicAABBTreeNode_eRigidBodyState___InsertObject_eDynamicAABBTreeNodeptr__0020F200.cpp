// eDynamicAABBTreeNode<eRigidBodyState>::InsertObject(eRigidBodyState *)
// Address: 0x0020f200, Size: 92B, Obj: eAll_psp.obj
// Symbol: __0fUeDynamicAABBTreeNode76PeRigidBodyState_MInsertObjectP9BA_v
//
// Intrusive circular doubly-linked-list insert. The list nodes (the template
// parameter T == eRigidBodyState) carry prev/next/owner link fields at offsets
// 0xB0/0xB4/0xB8; the tree node holds the list head at 0x48 and a u16 count at
// 0x4C. ODR-safe: types declared locally, never added to a shared header.

struct eRigidBodyState {
    char pad_b0[0xB0];
    eRigidBodyState *prev;  // 0xB0
    eRigidBodyState *next;  // 0xB4
    void *owner;            // 0xB8
};

template <class T>
class eDynamicAABBTreeNode {
public:
    char pad48[0x48];
    T *head;               // 0x48
    unsigned short count;  // 0x4C

    void InsertObject(T *obj);
};

template <class T>
void eDynamicAABBTreeNode<T>::InsertObject(T *obj) {
    T *h = this->head;
    if (h != 0) {
        obj->prev = h->prev;
        obj->next = this->head;
        __asm__ volatile("" ::: "memory");
        obj->prev->next = obj;
        obj->next->prev = obj;
        obj->owner = this;
        this->count = (unsigned short)(this->count + 1);
        return;
    }
    this->head = obj;
    obj->next = obj;
    obj->prev = obj;
    obj->owner = this;
    this->count = (unsigned short)(this->count + 1);
}

// Force instantiation of the eRigidBodyState specialization.
template void eDynamicAABBTreeNode<eRigidBodyState>::InsertObject(eRigidBodyState *);
