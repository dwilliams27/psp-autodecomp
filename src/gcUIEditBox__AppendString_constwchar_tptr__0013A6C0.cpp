class gcUIEditBox {
public:
    char pad_0[0xD0];
    wchar_t *mString;
    char pad_D4[0x48];
    int mCursorPos;
    void AppendString(const wchar_t *string);
    void InsertString(const wchar_t *string, int pos);
};
extern int cStrLength(const wchar_t *, int);
void gcUIEditBox::AppendString(const wchar_t *string) {
    if (string == 0) return;
    int empty = 1;
    wchar_t *str = mString;
    if (str != 0) {
        empty = (*((int *)str - 1) & 0x3FFFFFFF) == 0;
    }
    if (empty) return;
    InsertString(string, cStrLength(str, empty));
}
