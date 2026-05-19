// ODR-WARNING: split TU for gcFormatString::Get. Keep local declarations
// minimal so this retry source does not perturb matched gcFormatString.cpp.

class gcStringTable;
class gcString;

template <class T>
class cHandleT {
public:
    int mIndex;
};

template <class T>
class cSubHandleT {
public:
    int mIndex;
};

template <class T, class U>
class cHandlePairT {
public:
    cHandleT<T> mHandle;
    U mSubHandle;
};

class cBaseArray {
public:
    void *mData;
    void *mOwner;
};

class gcStringValue {
public:
    int _v0;
    int _v4;
};

class cStringBufferW {
public:
    wchar_t *start;
    wchar_t *end;
};

class cParserW {
public:
    char flag;
    char pad[3];
    const wchar_t *pos;
    int NextAny(const wchar_t *, const wchar_t *);
    void ValueAny(wchar_t *, int, const wchar_t *, const wchar_t *);
};

class gcString {
public:
    char pad[0x18];
    const wchar_t *mText;
};

class gcStringTable {
public:
    gcString *GetSubObject(cSubHandleT<gcString>, int) const;
};

class gcFormatString : public gcStringValue {
public:
    cHandlePairT<gcStringTable, cSubHandleT<gcString> > mPair;
    cBaseArray mArray0;
    cBaseArray mArray1;

    void Get(wchar_t *, int) const;
};

extern "C" gcString *gcStringTable_GetSubObject(gcStringTable *,
                                                cSubHandleT<gcString> *, int)
    asm("__0fNgcStringTableMGetSubObject6LcSubHandleT76IgcString_iK");
extern "C" void cStringBufferW_ctor(cStringBufferW *, wchar_t *, int)
    asm("__0oOcStringBufferWctPwi");
extern "C" void cStringBufferW_appendStr(cStringBufferW *, const wchar_t *)
    asm("__0oOcStringBufferWlsPCw");
extern "C" void cStringBufferW_appendChar(cStringBufferW *, wchar_t)
    asm("__0oOcStringBufferWlsw");
extern "C" void cStringBufferW_skipToEnd(cStringBufferW *)
    asm("__0fOcStringBufferWJSkipToEndv");

void cItoA(int, wchar_t *);
int cAtoI(const wchar_t *);
extern gcStringTable *D_00038890[];

struct FloatDispatchEntry {
    short offset;
    short pad;
    float (*fn)(void *);
};

struct WStringDispatchEntry {
    short offset;
    short pad;
    void (*fn)(void *, wchar_t *, int);
};

struct gcFormatStringGetLocals {
    cStringBufferW out;
    cParserW parser;
    wchar_t value[8];
    wchar_t number[16];
    cSubHandleT<gcString> sub;
    const wchar_t *typeChars;
};

// ── gcFormatString::Get(wchar_t *, int) const @ 0x0027c630 ──
void gcFormatString::Get(wchar_t *buf, int size) const {
    gcFormatStringGetLocals locals;
    int handle = this->mPair.mHandle.mIndex;
    gcStringTable *table;
    if (handle == 0) {
        table = 0;
    } else {
        gcStringTable *candidate = D_00038890[handle & 0xFFFF];
        table = 0;
        if (candidate != 0) {
            int candidateHandle = *(int *)((char *)candidate + 0x30);
            if (candidateHandle == handle) {
                table = candidate;
            }
        }
    }

    gcString *string = 0;
    if (table != 0) {
        locals.sub.mIndex = this->mPair.mSubHandle.mIndex;
        string = gcStringTable_GetSubObject(table, &locals.sub, 0);
    }

    if (string != 0) {
        cStringBufferW_ctor(&locals.out, buf, size);
        const wchar_t *text = string->mText;
        if (text == 0) {
            text = (const wchar_t *)0x36DAF4;
        }

        locals.parser.flag = 0;
        locals.parser.pos = text;

        if (*locals.parser.pos != 0) {
            const wchar_t *valueChars = (const wchar_t *)0x36DFEC;
            const wchar_t *endChars = (const wchar_t *)0x36DFF0;
            const wchar_t *closeChars = (const wchar_t *)0x36E008;
            locals.typeChars = (const wchar_t *)0x36DFFC;

            do {
                const wchar_t *p = locals.parser.pos;
                wchar_t ch = *p;
                if (ch != 0x7B) {
                    goto literal_char;
                }

                if (*p != 0) {
                    locals.parser.pos = p + 1;
                }

                locals.parser.ValueAny(locals.value, 8, valueChars, endChars);
                locals.parser.NextAny(valueChars, endChars);
                int index = cAtoI(locals.value);
                int type = locals.parser.NextAny(locals.typeChars, endChars);

                if (type < 106) {
                    if (type >= 105) {
                        if (index >= 0) {
                            void **array = (void **)this->mArray0.mData;
                            int count = 0;
                            if (array != 0) {
                                count = ((int *)array)[-1];
                            }
                            if (index < count) {
                                void *obj = array[index];
                                if (obj != 0) {
                                    FloatDispatchEntry *entry =
                                        (FloatDispatchEntry *)(*(char **)((char *)obj + 4) + 0x70);
                                    float f = entry->fn((char *)obj + entry->offset);
                                    cItoA((int)f, locals.number);
                                    cStringBufferW_appendStr(&locals.out, locals.number);
                                }
                            }
                        }
                        locals.parser.NextAny(closeChars, endChars);
                    }
                } else if (type == 0x73) {
                    if (index >= 0) {
                        void **array = (void **)this->mArray1.mData;
                        int count = 0;
                        if (array != 0) {
                            count = ((int *)array)[-1];
                        }
                        if (index < count) {
                            void *obj = array[index];
                            if (obj != 0) {
                                WStringDispatchEntry *entry =
                                    (WStringDispatchEntry *)(*(char **)((char *)obj + 4) + 0x70);
                                int byteCount = (char *)locals.out.end - (char *)locals.out.start;
                                entry->fn((char *)obj + entry->offset, locals.out.start, byteCount / 2);
                                cStringBufferW_skipToEnd(&locals.out);
                            }
                        }
                    }
                    locals.parser.NextAny(closeChars, endChars);
                }
                goto next_char;

literal_char:
                cStringBufferW_appendChar(&locals.out, ch);
                p = locals.parser.pos;
                if (*p != 0) {
                    locals.parser.pos = p + 1;
                }
next_char:
                ;
            } while (*locals.parser.pos != 0);
        }
    }
}
