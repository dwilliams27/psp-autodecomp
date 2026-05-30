// cGetLanguageAndButton(int *, int *)
// Address: 0x00001bb0  Size: 192B  Obj: cAll_psp.obj
// Symbol: __0FVcGetLanguageAndButtonPiTB
//
// Free function. Reads a file-scope global selector (relocation masked by
// compare_func) and dispatches via a 12-entry jump table (cases 0..11,
// SNC jump-table strategy for 8+ dense cases, docs/research/snc-loop-switch.md).
// *pButton is always set to 1 (the `li a2,1` is hoisted before the jr); each
// case stores a language id into *pLanguage. Case bodies appear in source
// order in .text; the jump table maps switch value -> body.

static unsigned int g_LanguageSelector;

void cGetLanguageAndButton(int *pLanguage, int *pButton)
{
    switch (g_LanguageSelector)
    {
    case 0:
        *pLanguage = 1;
        *pButton = 1;
        return;
    case 1:
        *pLanguage = 3;
        *pButton = 1;
        return;
    case 2:
        *pLanguage = 2;
        *pButton = 1;
        return;
    case 3:
        *pLanguage = 4;
        *pButton = 1;
        return;
    case 4:
        *pLanguage = 6;
        *pButton = 1;
        return;
    case 5:
        *pLanguage = 5;
        *pButton = 1;
        return;
    case 8:
        *pLanguage = 0;
        *pButton = 1;
        return;
    case 11:
        *pLanguage = 8;
        *pButton = 1;
        return;
    case 10:
        *pLanguage = 9;
        *pButton = 1;
        return;
    default:
        *pLanguage = 1;
        *pButton = 1;
        return;
    }
}
