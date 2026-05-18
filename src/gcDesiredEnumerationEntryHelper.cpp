// gcDesiredEnumerationEntryHelper — gcAll_psp.obj
//   0x0010e450  Write(cWriteBlock &) const           (104B)
//   0x0010e958  GetText(char *, bool) const          (372B)

class cFile;

class cWriteBlock {
public:
    cFile *_file;
    int _pos;
    cWriteBlock(cFile &, unsigned int);
    void Write(char);
    void Write(unsigned int);
    void End(void);
};

class cHandle {
public:
    int mId;
    void Write(cWriteBlock &) const;
};

class cStr {
public:
    char _data[256];
    cStr() {}
    cStr(const char *);
};

extern "C" void cHandlePairT_GetName(void *, char *, int, int);
extern "C" void cStrCat(char *, const char *);
extern "C" void cStrAppend(char *, const char *, ...);

struct gcDesiredEnumerationEntryHelper {
    char mField0;
    char mField1;
    cHandle mField4;
    unsigned int mField8;
    void Write(cWriteBlock &) const;
    void GetText(char *, bool) const;
};

// 0x0010e450 — Write(cWriteBlock &) const, 104B
void gcDesiredEnumerationEntryHelper::Write(cWriteBlock &outer) const {
    cWriteBlock wb(*outer._file, 1);
    wb.Write(mField0);
    wb.Write(mField1);
    mField4.Write(wb);
    wb.Write(mField8);
    wb.End();
}

// 0x0010e958 — GetText(char *, bool) const, 372B
void gcDesiredEnumerationEntryHelper::GetText(char *buf, bool flag) const {
    cStr text;
    if (mField0 == 1) {
        text._data[0] = '\0';
        if (mField1 == 0) {
            cHandlePairT_GetName((char *)this + 4, text._data, flag, 1);
        } else {
            int hid = mField4.mId;
            void *entry;
            if (hid == 0) {
                entry = 0;
            } else {
                void **table = (void **)0x38890;
                void *cand = table[(unsigned short)hid];
                entry = 0;
                if (cand != 0 && *(int *)((char *)cand + 0x30) == hid) {
                    entry = cand;
                }
            }
            const char *src;
            if (entry != 0) {
                short v = *(short *)((char *)entry + 0x1C);
                int isZero = (v == 0);
                isZero &= 0xFF;
                if (isZero != 0) {
                    src = (const char *)0x36DAB8;
                } else {
                    src = (const char *)((char *)entry + 8);
                }
            } else if (hid != 0) {
                src = (const char *)0x36DAC4;
            } else {
                src = (const char *)0x36DACC;
            }
            text = cStr(src);
        }
        cStrCat(buf, text._data);
    } else {
        cStrCat(buf, (const char *)0x36DAF0);
    }
    if (mField1 != 0) {
        cStrAppend(buf, (const char *)0x36DCB8, (const char *)0x36DAF0);
    }
}
