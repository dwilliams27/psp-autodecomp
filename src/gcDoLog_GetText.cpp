// ODR-WARNING: this TU intentionally redeclares gcDoLog with a minimal
// member list for a method absent from the canonical header. Do not include
// gcDoEntitySendMessage.h here unless all matched gcDoLog siblings are
// rechecked.
class gcDoLog {
public:
    char _pad[0x08];
    int mFlags;
    int mNumChildren;
    struct {
        char *mData;
    } mText;
    int *mChildren;

    void GetText(char *) const;
};

void cStrCat(char *, const char *);

void gcDoLog::GetText(char *buf) const {
    int numChildren = mNumChildren;
    if (numChildren == 0) {
        int keepDefault = 1;
        if (mFlags & 1) {
            keepDefault = 0;
        }
        if (keepDefault != 0) {
            cStrCat(buf, (const char *)0x36DFE8);
        }

        char *text = mText.mData;
        int hasText = 0;
        if (text != 0) {
            hasText = (*(int *)(text - 4) & 0x3FFFFFFF) > 0;
        }
        char *append;
        if (hasText != 0) {
            append = text;
        } else {
            append = (char *)0x36D944;
        }
        cStrCat(buf, append);
    } else {
        cStrCat(buf, (const char *)0x36DAF0);
        cStrCat(buf, (const char *)0x36ECB0);

        char *text = mText.mData;
        int hasText = 0;
        if (text != 0) {
            hasText = (*(int *)(text - 4) & 0x3FFFFFFF) > 0;
        }
        char *append;
        if (hasText != 0) {
            append = text;
        } else {
            append = (char *)0x36D944;
        }
        cStrCat(buf, append);

        cStrCat(buf, (const char *)0x36ECB4);

        char *suffix;
        if (*mChildren != 0) {
            suffix = (char *)0x36DAD8;
        } else {
            suffix = (char *)0x36DCEC;
        }
        cStrCat(buf, suffix);
    }
}
