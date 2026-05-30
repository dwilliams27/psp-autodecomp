// gcDesiredObjectHelper::GetPrimaryText(gcDesiredObjectHelper::gcPrimary) static
// Address: 0x001251c8, Size: 12B
// Obj: gcAll_psp.obj
//
// Leaf function: returns a baked-in pointer constant (string-literal address
// in rodata; the relocation is masked by compare_func). Sibling pattern:
// gcDesiredEntityHelper::GetRelationshipText (MATCHED).

class gcDesiredObjectHelper {
public:
    enum gcPrimary {};
    static const char *GetPrimaryText(gcPrimary);
};

const char *gcDesiredObjectHelper::GetPrimaryText(gcPrimary)
{
    return (const char *)0x36DAF0;
}
