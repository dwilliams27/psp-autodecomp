// gcDesiredUIWidgetHelper::GetPrimaryText(gcDesiredUIWidgetHelper::gcPrimary) static
// Address: 0x0012e9a0, Size: 216B
// Obj: gcAll_psp.obj
//
// Leaf function: switch over gcPrimary returning baked-in string-literal
// pointer constants in rodata. compare_func masks the relocations, so the
// concrete pointer values are irrelevant to the byte match; only the
// switch/jump-table structure matters. Sibling pattern (MATCHED):
// gcDesiredEntityHelper::GetRelationshipText, gcDesiredObjectHelper::GetPrimaryText.
//
// Switch codegen note: the original keeps a full 15-entry jump table (sltiu <15)
// where entry[14] and the out-of-range path BOTH target one full default block
// (its own lui v0,0x37). pspsnc collapses this two ways from naive C:
//   - "case 14: default: return X;" merges 14 into default AND shrinks the bounds
//     to sltiu <14 (212B, 14-entry table) — loses the case-14 entry.
//   - "case 14: return X; default: return X;" emits two separate full blocks plus
//     a tail-merged stub (224B) — extra block.
// Routing case 14 via "goto" to a label on the default block keeps all 15 jump
// entries while pointing entry[14] at the single shared full default block,
// reproducing the original 216B layout exactly.

class gcDesiredUIWidgetHelper {
public:
    enum gcPrimary {};
    static const char *GetPrimaryText(gcPrimary);
};

const char *gcDesiredUIWidgetHelper::GetPrimaryText(gcPrimary p)
{
    switch (p) {
    case 0:  return (const char *)0x36DACC;
    case 1:  return (const char *)0x36DCF0;
    case 2:  return (const char *)0x36DCFC;
    case 3:  return (const char *)0x36DD0C;
    case 4:  return (const char *)0x36DD1C;
    case 5:  return (const char *)0x36DD2C;
    case 6:  return (const char *)0x36DD38;
    case 7:  return (const char *)0x36DD48;
    case 8:  return (const char *)0x36DD58;
    case 9:  return (const char *)0x36DD68;
    case 10: return (const char *)0x36DD78;
    case 11: return (const char *)0x36DD88;
    case 12: return (const char *)0x36DFA8;
    case 13: return (const char *)0x36DFBC;
    case 14: goto def;
    default: def: return (const char *)0x36DAF0;
    }
}
