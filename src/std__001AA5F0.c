/* std @ 0x001aa5f0, 88B, gMain_psp.obj
 * Leaf constructor-style init. Symbol literally named "std".
 * C TU: function name "std" is a valid identifier in C and emits
 * the unmangled symbol "std".
 */

struct Obj {
    int   unk0;     /* 0  */
    int   unk4;     /* 4  */
    int   unk8;     /* 8  */
    short unkC;     /* 12 */
    short unkE;     /* 14 */
    int   unk10;    /* 16 */
    int   pad14;    /* 20 */
    int   unk18;    /* 24 */
    void *unk1C;    /* 28 */
    void *unk20;    /* 32 */
    void *unk24;    /* 36 */
    void *unk28;    /* 40 */
    void *unk2C;    /* 44 */
    int   pad30[9]; /* 48..83 */
    int   unk54;    /* 84 (0x54) */
};

extern void ptr1(void);
extern void ptr2(void);
extern void ptr3(void);
extern void ptr4(void);

void std(struct Obj *o, int a, int b, int d)
{
    o->unk0  = 0;
    o->unk4  = 0;
    o->unk8  = 0;
    o->unkC  = (short)a;
    o->unkE  = (short)b;
    o->unk10 = 0;
    o->unk18 = 0;
    o->unk1C = o;
    o->unk20 = (void*)&ptr1;
    o->unk24 = (void*)&ptr2;
    o->unk28 = (void*)&ptr3;
    o->unk2C = (void*)&ptr4;
    o->unk54 = d;
}
