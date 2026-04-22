#include "eCollisionConstraint.h"

class cMemPool {
public:
    static void *GetPoolFromPtr(const void *);
};

extern eCollisionConstraint *gCollisionConstraintFreeList;  // 0x37D320
extern void **gCollisionConstraintPool;  // 0x37D324
extern void *gPhysicsConfig;  // 0x37D0C8

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

extern char eCollisionConstraintLabel[];

int eCollisionConstraint::Initialize(cMemPool *pool) {
    void **s0 = 0;

    // Allocate 4-byte cell
    void *block = *(void **)((char *)pool + 0x24);
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void **cell = (void **)entry->fn((char *)block + off, 4, 0, 0, -1);
    if (cell != 0) {
        *cell = 0;
        s0 = cell;
    }
    gCollisionConstraintPool = s0;

    int count = *(int *)((char *)gPhysicsConfig + 0x68);

    // Zero a default entry on the stack
    eCollisionConstraint def;
    *(int *)((char *)&def + 0) = 0;
    *(int *)((char *)&def + 4) = 0;
    *(int *)((char *)&def + 8) = 0;
    *(int *)((char *)&def + 12) = 0;
    *(int *)((char *)&def + 16) = 0;

    int capacity = 0;
    void *data = *s0;
    if (data != 0) {
        capacity = *((int *)data - 1) & 0x3FFFFFFF;
    }

    if (capacity == count) {
        goto buildFreeList;
    }

    {
        void *newData;
        if (count > 0) {
            void *pool2 = cMemPool::GetPoolFromPtr(s0);
            void *block2 = *(void **)((char *)pool2 + 0x24);
            char *allocTable2 = *(char **)((char *)block2 + 0x1C);
            AllocEntry *entry2 = (AllocEntry *)(allocTable2 + 0x28);
            short off2 = entry2->offset;
            int *header = (int *)entry2->fn((char *)block2 + off2, count * 20 + 4, 4, (int)eCollisionConstraintLabel, 0x112);
            *header = count;
            newData = (void *)((char *)header + 4);
        } else {
            newData = 0;
        }

        if (newData != 0) goto doCopy;
        if (count <= 0) goto doCopy;
        goto buildFreeList;

    doCopy:
        {
            int i = 0;
            int byteOff = 0;
            char *dst = (char *)newData + byteOff;
            while (i < count) {
                if (dst != 0) {
                    char *src;
                    if (i < capacity) {
                        src = (char *)(*s0) + byteOff;
                    } else {
                        src = (char *)&def;
                    }
                    *(eCollisionConstraint *)dst = *(eCollisionConstraint *)src;
                }
                i++;
                byteOff += 20;
                dst = (char *)newData + byteOff;
            }

            void *oldData = *s0;
            void *oldSave = *s0;
            int oldCap = 0;
            if (oldData != 0) {
                oldCap = *((int *)oldData - 1) & 0x3FFFFFFF;
            }
            for (int j = 0; j < oldCap; j++) {}

            if (oldSave != 0) {
                int *allocHeader = (int *)oldSave - 1;
                if (allocHeader != 0) {
                    void *pool3 = cMemPool::GetPoolFromPtr(allocHeader);
                    void *block3 = *(void **)((char *)pool3 + 0x24);
                    DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block3 + 0x1C) + 0x30);
                    short off3 = rec->offset;
                    rec->fn((char *)block3 + off3, allocHeader);
                }
                *s0 = 0;
            }
            *s0 = newData;
        }
    }

buildFreeList:
    {
        int k = 0;
        int offset = 0;
        for (; k < *(int *)((char *)gPhysicsConfig + 0x68); k++) {
            ((eCollisionConstraint *)((char *)(*gCollisionConstraintPool) + offset))->mNext = gCollisionConstraintFreeList;
            gCollisionConstraintFreeList = (eCollisionConstraint *)((char *)(*gCollisionConstraintPool) + offset);
            offset += 20;
        }
    }

    return 1;
}

void eCollisionConstraint::Free(eCollisionConstraint *p) {
    p->mNext = gCollisionConstraintFreeList;
    gCollisionConstraintFreeList = p;
}

eCollisionConstraint *eCollisionConstraint::Allocate(const eContactFeature &feature) {
    eCollisionConstraint *node = gCollisionConstraintFreeList;
    if (node) {
        gCollisionConstraintFreeList = node->mNext;
        __asm__ volatile("" ::: "memory");
        node->mValue = 0.0f;
        eContactFeature *dst = &node->mFeature;
        *dst = feature;
        node->mFlags = 0;
        return node;
    }
    return 0;
}
