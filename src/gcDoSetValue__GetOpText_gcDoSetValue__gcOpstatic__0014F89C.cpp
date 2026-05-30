// gcDoSetValue::GetOpText(gcDoSetValue::gcOp) static
// Address: 0x0014f89c, Size: 12B
// Obj: gcAll_psp.obj
//
// Single baked-in string pointer return (GetText-family, like
// gcUIControl::GetSpriteText default case). 12B = lui/jr/addiu.

class gcDoSetValue {
public:
    enum gcOp {};
    static const char *GetOpText(gcOp op);
};

const char *gcDoSetValue::GetOpText(gcOp op)
{
    return (const char *)0x36DAF0;
}
