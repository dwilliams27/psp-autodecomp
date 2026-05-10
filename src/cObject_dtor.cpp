// ODR-WARNING: split translation unit for cObject destructor only.
// Keep local declarations minimal to avoid register-allocation drift in matched siblings.

class cBase;
class cMemPool;
class cObject;
class cGroup;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteRec {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

struct TypeNode {
    char pad[0x1C];
    cType *parent;
};

struct TypeDispatchSlot {
    short offset;
    short _pad;
    cType *(*getType)(void *);
};

inline void operator delete(void *p) {
    if (p != 0) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRec *rec = (DeleteRec *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        char *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
}

class cObject {
public:
    ~cObject();
};

class cGroup {
public:
    void OnObjectDeleted(cObject *);
};

class cManager {
public:
    static void Remove(cObject *);
};

extern cType *D_000385DC;
extern cType *D_00040C94;

#pragma control sched=2

cObject::~cObject() {
    *(void **)((char *)this + 4) = (void *)0x37E920;

    unsigned int x = *(unsigned int *)((char *)this + 0x30);
    register int zero __asm__("$0");
    if (((zero == x) & 0xFF) == 0) {
        cGroup *grp = 0;
        void *owner = *(void **)this;

        if (owner != 0) {
            if (D_00040C94 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType(
                        (const char *)0x36C830, (const char *)0x36C838,
                        1, 0, 0, 0, 0, 0);
                }
                D_00040C94 = cType::InitializeType(0, 0, 4, D_000385DC, 0, 0, 0, 0);
            }

            cType *target = D_00040C94;
            char *vtab = *(char **)((char *)owner + 4);
            TypeDispatchSlot *slot = (TypeDispatchSlot *)(vtab + 8);
            short adj = slot->offset;
            cType *type = slot->getType((char *)owner + adj);

            int ok;
            if (target != 0) {
                goto have_target;
            }
            ok = 0;
            goto cast_done;

        have_target:
            if (type != 0) {
            loop:
                if (type == target) {
                    ok = 1;
                } else {
                    type = ((TypeNode *)type)->parent;
                    if (type != 0) {
                        goto loop;
                    }
                    goto invalid;
                }
            } else {
            invalid:
                ok = 0;
            }
        cast_done:
            if (ok != 0) {
                grp = (cGroup *)owner;
            }
        }

        if (grp != 0) {
            grp->OnObjectDeleted((cObject *)this);
        }
        cManager::Remove((cObject *)this);
    }

    if (this != 0) {
        *(void **)((char *)this + 4) = (void *)0x37E6A8;
    }
}
