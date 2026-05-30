// add_vec_new_or_delete_eh_stack_entry(an_eh_stack_entry *, an_array_alloc_eh_info *, int)
// Address: 0x001b9970, Size: 68B, Obj: gMain_psp.obj

struct an_eh_stack_entry {
    an_eh_stack_entry *next;   // 0
    unsigned char kind;        // 4
    void *info;                // 8
};

struct an_array_alloc_eh_info {
    int unk0;   // 0
    int unk4;   // 4
    int unk8;   // 8
    int unkC;   // C
    int count;  // 10
    int unk14;  // 14
    int unk18;  // 18
    int unk1C;  // 1C
    int unk20;  // 20
};

static an_eh_stack_entry *g_eh_stack_top = 0;

void add_vec_new_or_delete_eh_stack_entry(an_eh_stack_entry *entry,
                                          an_array_alloc_eh_info *info,
                                          int count)
{
    entry->next = g_eh_stack_top;
    g_eh_stack_top = entry;
    entry->kind = 4;
    entry->info = info;
    info->unk0 = 0;
    info->unk4 = 0;
    info->unk8 = 0;
    info->unkC = 0;
    info->count = count;
    info->unk14 = 0;
    info->unk18 = 0;
    info->unk1C = 0;
    info->unk20 = 0;
}
