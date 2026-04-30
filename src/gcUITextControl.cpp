inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cFile;
class cMemPool;
class cType;
class eSprite;

template <class T> class cArray;
template <class T> class cArrayBase {
public:
    T *mData;
    cArrayBase &operator=(const cArrayBase &);
};

template <class T> class cHandleT {
public:
    int _data;
};

template <class T> T dcast(const cBase *);

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(short);
    void Write(unsigned short);
    void Write(int);
    void Write(unsigned int);
    void End(void);
};

class cHandle {
public:
    int _data;
    void Write(cWriteBlock &) const;
};

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct TextControlShortCursor {
    char pad[0x108];
    unsigned short value;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcUIControl {
public:
    char _pad[0xC0];

    gcUIControl(cBase *);
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

class gcUIWidget {
public:
    gcUIWidget &operator=(const gcUIWidget &);
};

class gcUITextControl : public gcUIControl {
public:
    float mFontSize;
    float mLineSpacing;
    int mText;
    int mStringTable;
    int mString;
    int mFont;
    int mSprites;
    int mSpriteCount;
    int mAlign;
    int mTextFlags;
    int mColor0;
    int mColor1;
    int mColor2;
    int mFieldF4;
    short mWidth;
    short mFieldFA;
    float mFieldFC;
    float mField100;
    int mField104;
    short mShorts[4];

    gcUITextControl(cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_0009990C;
extern cType *gcUIControl__s_pType__0042CF74;
extern cType *gcUITextControl__s_pType__0042CF78;

cBase *gcUITextControl::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcUITextControl *result = 0;
    gcUITextControl *obj = (gcUITextControl *)rec->fn(base, 0x110, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcUITextControl(parent);
        result = obj;
    }
    return (cBase *)result;
}

gcUITextControl::gcUITextControl(cBase *parent) : gcUIControl(parent) {
    *(void **)((char *)this + 4) = (void *)0x388E90;
    mText = 0;
    mStringTable = 0;
    mString = 0;
    mFont = 0;
    mSprites = 0;
    mSpriteCount = 0;
    mAlign = 4;
    mTextFlags = -1;
    mColor0 = -0x100;
    mColor1 = 0xFF808080;
    mColor2 = 0xFF000000;
    mFieldF4 = 0;
    mWidth = 0x140;
    mFieldFA = 0;
    mFieldFC = 0.0f;
    mField100 = 0.0f;
    mField104 = 0;

    int i = 0;
    TextControlShortCursor *p = (TextControlShortCursor *)this;
    do {
        p->value = 0;
        i += 1;
        p = (TextControlShortCursor *)((char *)p + 2);
    } while (i < 4);

    mFontSize = 32.0f;
    mLineSpacing = 4.0f;
}

const cType *gcUITextControl::GetType(void) const {
    if (gcUITextControl__s_pType__0042CF78 == 0) {
        if (gcUIControl__s_pType__0042CF74 == 0) {
            if (D_0009990C == 0) {
                if (D_000385E0 == 0) {
                    if (D_000385DC == 0) {
                        D_000385DC = cType::InitializeType(
                            (const char *)0x36D894, (const char *)0x36D89C,
                            1, 0, 0, 0, 0, 0);
                    }
                    D_000385E0 = cType::InitializeType(
                        0, 0, 2, D_000385DC,
                        (cBase *(*)(cMemPool *, cBase *))0x1C3C58, 0, 0, 0);
                }
                D_0009990C = cType::InitializeType(
                    0, 0, 0x84, D_000385E0, 0, 0, 0, 0);
            }
            gcUIControl__s_pType__0042CF74 = cType::InitializeType(
                0, 0, 0x201, D_0009990C, gcUIControl::New, 0, 0, 0);
        }
        gcUITextControl__s_pType__0042CF78 = cType::InitializeType(
            0, 0, 0x200, gcUIControl__s_pType__0042CF74,
            gcUITextControl::New, 0, 0, 0);
    }
    return gcUITextControl__s_pType__0042CF78;
}

void gcUITextControl::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const gcUIControl *)this)->Write(file);
    ((const cHandle *)((const char *)this + 0xC8))->Write(wb);
    wb.Write(*(const unsigned int *)((const char *)this + 0xCC));

    int count = 0;
    const int mask = 0x3FFFFFFF;
    const int *text = *(const int *const *)((const char *)this + 0xD0);
    if (text != 0) {
        count = text[-1] & mask;
    }
    wb.Write(count);

    ((const cHandle *)((const char *)this + 0xF4))->Write(wb);
    wb.Write(*(const int *)((const char *)this + 0xE0));
    wb.Write(*(const unsigned int *)((const char *)this + 0xE4));
    wb.Write(*(const unsigned int *)((const char *)this + 0xE8));
    wb.Write(*(const unsigned int *)((const char *)this + 0xEC));
    wb.Write(*(const unsigned int *)((const char *)this + 0xF0));
    wb.Write(*(const short *)((const char *)this + 0xF8));
    wb.Write(*(const unsigned short *)((const char *)this + 0xFA));

    count = 0;
    const int *widths = *(const int *const *)((const char *)this + 0xD4);
    if (widths != 0) {
        count = widths[-1] & mask;
    }
    wb.Write(count);

    count = 0;
    const int **lines = *(const int **const *)((const char *)this + 0xD8);
    if (lines != 0) {
        count = ((const int *)lines)[-1] & mask;
    }
    wb.Write(count);

    int i = 0;
    int offset = 0;
    do {
        int lineCount = 0;
        const int **lineArray = *(const int ***)((const char *)this + 0xD8);
        if (lineArray != 0) {
            lineCount = ((const int *)lineArray)[-1] & mask;
        }
        if (i >= lineCount) {
            break;
        }

        const int **slot = (const int **)((const char *)lineArray + offset);
        const int *line = *slot;
        int lineLen = 0;
        if (line != 0) {
            lineLen = line[-1] & mask;
        }
        wb.Write(lineLen);
        i += 1;
        offset += 4;
    } while (1);

    count = 0;
    const cHandle *spriteArray =
        *(const cHandle *const *)((const char *)this + 0xDC);
    if (spriteArray != 0) {
        count = ((const int *)spriteArray)[-1] & mask;
    }
    wb.Write(count);

    const cHandle *sprites = *(const cHandle *const *)((const char *)this + 0xDC);
    int spriteCount = 0;
    if (sprites != 0) {
        spriteCount = ((const int *)sprites)[-1] & mask;
    }
    int spriteIndex = 0;
    int spriteOffset = 0;
    if (spriteIndex < spriteCount) {
        const cHandle *handle =
            (const cHandle *)((const char *)sprites + spriteOffset);
        do {
            handle->Write(wb);
            spriteIndex += 1;
            handle = (const cHandle *)((const char *)handle + 4);
            spriteOffset += 4;
        } while (spriteIndex < spriteCount);
    }

    wb.End();
}

void gcUITextControl::AssignCopy(const cBase *base) {
    gcUITextControl *other = dcast<gcUITextControl *>(base);
    ((gcUIWidget *)this)->operator=(*(const gcUIWidget *)other);
    *(int *)((char *)this + 0xB0) = *(const int *)((const char *)other + 0xB0);

    int i = 0;
    int *dst = (int *)((char *)this + 0xB4);
    const int *src = (const int *)((const char *)other + 0xB4);
    do {
        i += 1;
        *dst = *src;
        dst += 1;
        src += 1;
    } while (i < 3);

    *(float *)((char *)this + 0xC0) = *(const float *)((const char *)other + 0xC0);
    const int *srcC8 = (const int *)((const char *)other + 0xC8);
    *(float *)((char *)this + 0xC4) = *(const float *)((const char *)other + 0xC4);
    int *dstC8 = (int *)((char *)this + 0xC8);
    unsigned int valueC8 = *srcC8;
    int *dstCC = (int *)((char *)this + 0xCC);
    *dstC8 = valueC8;
    const int *srcCC = (const int *)((const char *)other + 0xCC);
    *dstCC = *srcCC;

    ((cArrayBase<wchar_t> *)((char *)this + 0xD0))->operator=(
        *(const cArrayBase<wchar_t> *)((const char *)other + 0xD0));
    ((cArrayBase<float> *)((char *)this + 0xD4))->operator=(
        *(const cArrayBase<float> *)((const char *)other + 0xD4));
    ((cArrayBase<cArray<wchar_t> > *)((char *)this + 0xD8))->operator=(
        *(const cArrayBase<cArray<wchar_t> > *)((const char *)other + 0xD8));
    ((cArrayBase<cHandleT<eSprite> > *)((char *)this + 0xDC))->operator=(
        *(const cArrayBase<cHandleT<eSprite> > *)((const char *)other + 0xDC));

    int valueE0 = *(const int *)((const char *)other + 0xE0);
    *(int *)((char *)this + 0xE0) = valueE0;
    const int *srcE4 = (const int *)((const char *)other + 0xE4);
    int *dstE4 = (int *)((char *)this + 0xE4);
    const int *srcE8 = (const int *)((const char *)other + 0xE8);
    *dstE4 = *srcE4;
    int *dstE8 = (int *)((char *)this + 0xE8);
    const int *srcEC = (const int *)((const char *)other + 0xEC);
    *dstE8 = *srcE8;
    int *dstEC = (int *)((char *)this + 0xEC);
    const int *srcF0 = (const int *)((const char *)other + 0xF0);
    *dstEC = *srcEC;
    int *dstF0 = (int *)((char *)this + 0xF0);
    const int *srcF4 = (const int *)((const char *)other + 0xF4);
    int *dstF4 = (int *)((char *)this + 0xF4);
    *dstF0 = *srcF0;
    int valueF4 = *srcF4;
    *dstF4 = valueF4;
    *(short *)((char *)this + 0xF8) = *(const short *)((const char *)other + 0xF8);
    *(unsigned short *)((char *)this + 0xFA) =
        *(const unsigned short *)((const char *)other + 0xFA);
    *(float *)((char *)this + 0xFC) = *(const float *)((const char *)other + 0xFC);
    *(float *)((char *)this + 0x100) = *(const float *)((const char *)other + 0x100);
    *(int *)((char *)this + 0x104) = *(const int *)((const char *)other + 0x104);

    int j = 0;
    TextControlShortCursor *dstShorts = (TextControlShortCursor *)this;
    const TextControlShortCursor *srcShorts =
        (const TextControlShortCursor *)other;
    do {
        dstShorts->value = *(const unsigned short *)&srcShorts->value;
        j += 1;
        srcShorts = (const TextControlShortCursor *)((const char *)srcShorts + 2);
        dstShorts = (TextControlShortCursor *)((char *)dstShorts + 2);
    } while (j < 4);
}
