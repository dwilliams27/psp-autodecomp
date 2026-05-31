// eFont::ePrinter::PrintString(const wchar_t *) @ 0x00039b50
// Mangled: __0f5FeFontIePrinterLPrintStringPCw
//
// Split-TU reconstruction. ePrinter is a class nested inside eFont.
// Self-contained class declaration (do NOT modify include/eFont.h).

class eSprite;
class eFontCharKern;

class eFont {
public:
    class ePrinter {
    public:
        char pad00[0x54];   // +0x00
        int  mField54;      // +0x54

        int  PrintString(const wchar_t *str);
        int  PrintNewLine();
        int  PrintCode(wchar_t c, const wchar_t *&p);
        int  PrintChar(wchar_t c, const eSprite *spr);
    };
};

int eFont::ePrinter::PrintString(const wchar_t *str)
{
    wchar_t c = *str;
    while (c != 0) {
        wchar_t arg = c;
        if (c == 0xA) {
            if (!PrintNewLine())
                return 0;
        } else if (c == 0xD) {
            // skip
        } else if (c == 0x7B) {
            if (!PrintCode(arg, str))
                return 0;
        } else {
            if (!PrintChar(arg, 0))
                return 0;
        }
        ++str;
        ++mField54;
        c = *str;
    }

    return 1;
}
