// gcEntity::OnMemPoolResetAll(const cMemPool *, unsigned int) static @ 0x00114828
//
// Iterates the global circular list of live entities (head at *0x37D86C,
// next-pointer at entity+0x228). For each entity, dispatches through a
// per-type record pointed to by entity+4: at record+0x58 a signed short
// gives the sub-object byte offset, and record+0x5C holds the handler
// function pointer, called as fn(entity + offset, pool, flags).
//
// Split-TU pattern: gcEntity is declared locally (only the members this
// translation unit needs) so SNC mangles OnMemPoolResetAll natively as
// __0fIgcEntityROnMemPoolResetAllPC6IcMemPoolUiT.

class cMemPool;

class gcEntity {
public:
    static void OnMemPoolResetAll(const cMemPool *, unsigned int);
};

struct EntityTypeRec {
    char pad[0x58];
    short offset;   // +0x58
    short _pad;
    void (*fn)(void *, const cMemPool *, unsigned int);  // +0x5C
};

struct gcEntityNode {
    char pad0[0x04];             // +0x00 (cObject vtable etc.)
    EntityTypeRec *rec;          // +0x04
    char pad1[0x220];
    gcEntityNode *next;          // +0x228
};

void gcEntity::OnMemPoolResetAll(const cMemPool *pool, unsigned int flags) {
    // The list head lives at 0x37D86C. The original keeps 0x380000 in a
    // base register (lui) and addresses the head with displacement -10132
    // (= 0x37D86C - 0x380000), reusing that base for the in-loop reload.
    gcEntityNode **head = (gcEntityNode **)0x380000;
    gcEntityNode *node = head[-2533];
    if (node != 0) {
        do {
            EntityTypeRec *rec = node->rec;
            EntityTypeRec *slot = (EntityTypeRec *)((char *)rec + 0x58);
            short off = *(short *)slot;
            void (*fn)(void *, const cMemPool *, unsigned int) =
                *(void (**)(void *, const cMemPool *, unsigned int))((char *)slot + 4);
            fn((char *)node + off, pool, flags);
            node = node->next;
        } while (node != head[-2533]);
    }
}
