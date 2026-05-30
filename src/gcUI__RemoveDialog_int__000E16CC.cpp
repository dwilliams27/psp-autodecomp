// gcUI::RemoveDialog(int) @ 0x000e16cc, 176B, gcAll_psp.obj
// mangled: __0fEgcUIMRemoveDialogi
// RemoveDialog isn't declared in include/gcUI.h; declare the class locally (split-TU).

class gcUIDialog;

class gcUI {
public:
    char pad50[0x54];          // 0x00..0x53
    int  dialogs[0x20];        // 0x54: active dialog array (4-byte stride)
    char padD4[0xD4 - (0x54 + 0x20 * 4)];
    int  dialogCount;          // 0xD4
    char padDC[0xDC - (0xD4 + 4)];
    int  destroyList[0x20];    // 0xDC
    char pad15C[0x15C - (0xDC + 0x20 * 4)];
    int  destroyCount;         // 0x15C

    void RemoveDialog(int index);
};

void gcUI::RemoveDialog(int index)
{
    int removed = dialogs[index];

    int i = index + 1;
    if (i < 0x20) {
        int v;
    loop_top:
        v = dialogs[i];
        if (v != 0) {
            dialogs[i - 1] = v;
            ++i;
            if (i < 0x20) {
                goto loop_top;
            }
        }
    }

    int count = dialogCount;
    dialogs[count] = 0;
    dialogCount = count - 1;

    int flags = *(int *)((char *)removed + 0x54);
    if ((((flags & 8) != 0) & 0xFF) == 0 && (((flags & 4) != 0) & 0xFF)) {
        destroyList[destroyCount++] = removed;
        *(int *)((char *)removed + 0x54) = *(int *)((char *)removed + 0x54) | 8;
    }
}
