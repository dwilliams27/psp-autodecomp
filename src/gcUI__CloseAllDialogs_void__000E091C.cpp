// gcUI::CloseAllDialogs(void) @ 0x000e091c, 236B, gcAll_psp.obj
// mangled: __0fEgcUIPCloseAllDialogsv
// CloseAllDialogs isn't declared in include/gcUI.h; declare the class locally (split-TU).

class gcDoUIShowDialog;

class gcUIDialog {
public:
    void OnClosed(const gcDoUIShowDialog *, float *);
};

class gcUI {
public:
    char pad50[0x54];          // 0x00..0x53
    gcUIDialog *dialogs[0x20];  // 0x54: active dialog array (4-byte stride)
    char padD4[0xD4 - (0x54 + 0x20 * 4)];
    int  dialogCount;          // 0xD4

    void CloseAllDialogs(void);
};

extern int g_uiLockCounter;  // 0x38d7b0

void gcUI::CloseAllDialogs(void)
{
    --g_uiLockCounter;

    int pass = 0;
    int cont = 1;
loop_top:
    if (cont) {
        int count = dialogCount;
        if (count < 0) {
            goto done;
        }

        gcUIDialog *snapshot[0x21];

        int i;
        for (i = 0; count >= i; ++i) {
            snapshot[i + 1] = dialogs[i];
        }

        int count2 = dialogCount;
        int j;
        for (j = 0; count2 >= j; ++j) {
            gcUIDialog *d = snapshot[j + 1];
            if (d != 0) {
                d->OnClosed(0, 0);
            }
        }

        ++pass;
        cont = pass < 10;
        goto loop_top;
    }
done:;

    ++g_uiLockCounter;
}
