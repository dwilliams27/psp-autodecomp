inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cMemPool;
class cType;

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct TextControlShortCursor {
    char pad[0x108];
    short value;
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
