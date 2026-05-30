// gcUITextControl::SetTextColor(gcUITextControl::gcUITextColor, eColor)
// 0x0012366c

struct eColor {
    unsigned int value;
};

class gcUITextControl {
public:
    enum gcUITextColor {
        kColor0 = 0,
        kColor1 = 1,
        kColor2 = 2,
        kColor3 = 3,
    };
    char pad[0x110];
    void SetTextColor(gcUITextColor which, eColor color);
};

void gcUITextControl::SetTextColor(gcUITextColor which, eColor color) {
    switch (which) {
    case kColor0:
        *(eColor *)(pad + 0xE4) = color;
        break;
    case kColor1:
        *(eColor *)(pad + 0xE8) = color;
        break;
    case kColor2:
        *(eColor *)(pad + 0xEC) = color;
        break;
    case kColor3:
        *(eColor *)(pad + 0xF0) = color;
        break;
    }
}
