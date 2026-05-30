// gcUIControl::GetSpriteText(gcUIControl::gcUISprite) static
// Address: 0x00121f9c, Size: 76B

struct gcUIControl {
    enum gcUISprite {
        kSprite0 = 0,
        kSprite1 = 1,
        kSprite2 = 2,
    };
    static const char *GetSpriteText(gcUISprite a);
};

const char *gcUIControl::GetSpriteText(gcUISprite a) {
    switch (a) {
    case kSprite0:
        return (const char *)0x36DE78;
    case kSprite1:
        return (const char *)0x36DE80;
    case kSprite2:
        return (const char *)0x36DE8C;
    default:
        return (const char *)0x36DAF0;
    }
}
