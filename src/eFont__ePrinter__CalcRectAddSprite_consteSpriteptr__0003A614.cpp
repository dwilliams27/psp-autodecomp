// eFont::ePrinter::CalcRectAddSprite(const eSprite *)
// Address: 0x0003a614  Size: 260B  Obj: eAll_psp.obj
// Mangled: __0f5FeFontIePrinterRCalcRectAddSpritePC6HeSprite

extern "C" float fabsf(float);

class eSprite;

class eFont {
public:
    class ePrinter {
    public:
        // printer scale factors live at +108 / +112
        char pad0[108];
        float scaleX;   // +108
        float scaleY;   // +112

        int CalcRectAddChar(int a, int b, int x, int y, int w, float fy);
        int CalcRectAddSprite(const eSprite *spr);
    };
};

// const eSprite layout: floats at +76, +80, +100, +104
struct eSpriteLayout {
    char pad0[76];
    float f76;    // +76
    float f80;    // +80
    char pad1[16];
    float f100;   // +100
    float f104;   // +104
};

int eFont::ePrinter::CalcRectAddSprite(const eSprite *spr)
{
    if (spr != 0) {
        const eSpriteLayout *s = (const eSpriteLayout *)spr;

        float r[4];
        r[0] = s->f76;
        r[1] = s->f80;
        r[2] = s->f100;
        r[3] = s->f104;

        r[0] = r[0] * scaleX;
        r[1] = r[1] * scaleY;
        r[2] = r[2] * scaleX;
        r[3] = r[3] * scaleY;

        int rx = (int)(r[0] + fabsf(r[2]));
        float ry = r[1] + fabsf(r[3]);

        if (CalcRectAddChar(0, 0, rx, 0, -1, ry) != 0) {
            return 1;
        }
        return 0;
    }
    return 1;
}
