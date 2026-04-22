class cOutStream;
class cInStream;
class cFile;
class cMemPool;
class cBase;

extern "C" char *cStrCopy(char *, const char *);

// ─── gcTableColumn type descriptor / cast info ───
// Same pattern as cListSubscriber::GetName (see src/nwMsgConnectionAccepted.cpp).
struct gcTableColumnCastInfo {
    short mOffset;                        // +0
    short _pad;                           // +2
    cBase *(*mCast)(cBase *);             // +4
};

struct gcTableColumnTypeInfo {
    char _pad[8];                         // +0
    gcTableColumnCastInfo mCast;          // +8
};

struct cBaseWithName {
    char _pad[12];                        // +0
    const char *mName;                    // +12
};

// ─── cReadBlock (same layout as in gcPartialEntityController.cpp) ───
class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, int, bool);
    ~cReadBlock(void);
};

extern "C" {
    void cFile_SetCurrentPos(void *, unsigned int);
}

struct gcTableColumn {
    void *mOwner;                         // +0
    gcTableColumnTypeInfo *mTypeInfo;     // +4

    float Get(int) const;
    void *GetContainer(void) const;
    void Write(cOutStream &) const;
    void GetName(char *) const;
    void Read(cInStream &);
    int Read(cFile &, cMemPool *);
};

struct gcTableColumnStringTable {
    void Set(int, float);
};

// ─── existing stubs (already matched) ───
float gcTableColumn::Get(int) const {
    return 0.0f;
}

void *gcTableColumn::GetContainer(void) const {
    return 0;
}

void gcTableColumn::Write(cOutStream &) const {
}

void gcTableColumn::Read(cInStream &) {
}

void gcTableColumnStringTable::Set(int, float) {
}

// ============================================================
// Function 1: gcTableColumn::GetName(char *) const @ 0x00270ea8
// ============================================================
void gcTableColumn::GetName(char *dst) const {
    gcTableColumnCastInfo *ci = &mTypeInfo->mCast;
    cBase *obj = ci->mCast((cBase *)((char *)this + ci->mOffset));
    cStrCopy(dst, ((cBaseWithName *)obj)->mName);
}

// ============================================================
// Function 2: gcTableColumn::Read(cFile &, cMemPool *) @ 0x0012abc4
// ============================================================
int gcTableColumn::Read(cFile &file, cMemPool *) {
    int result = 1;
    cReadBlock rb(file, 1, true);
    if (rb._data[3] != 1) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return 0;
    }
    return result;
}

// ============================================================
// Function 3: cObject::cObject(cBase *) @ 0x00009d28
// ============================================================
extern char cObjectclassdesc[];  // at 0x0037E920

class cObject {
public:
    cBase *m0;                            // +0x00
    char *m4;                             // +0x04 (classdesc)
    char m8;                              // +0x08
    char _pad9[0x1C - 0x09];              // +0x09..0x1B unused
    short m1C;                            // +0x1C
    short m1E;                            // +0x1E
    int m20;                              // +0x20
    int m24;                              // +0x24
    short m28;                            // +0x28
    char _pad2A[2];                       // +0x2A..0x2B
    int m2C;                              // +0x2C
    int m30;                              // +0x30
    int m34;                              // +0x34
    int m38;                              // +0x38
    int m3C;                              // +0x3C
    int m40;                              // +0x40

    cObject(cBase *parent);
};

cObject::cObject(cBase *parent) {
    m0 = parent;
    m1C = 0;
    m1E = 0;
    m8 = 0;
    m4 = cObjectclassdesc;
    m20 = 0;
    m24 = 0;
    m28 = 0;
    m2C = 0;
    m30 = 0;
    m34 = 0;
    m38 = 0;
    m3C = 0;
    m40 = 0;
}

// ============================================================
// Function 4: eMaterial::Flip(void) static @ 0x0002c1d4
// ============================================================
struct eMaterialFlipVTableEntry {
    short offset;                         // +0
    short pad;                            // +2
    void (*fn)(void *);                   // +4
};

struct eMaterialFlipObject {
    char _pad[4];                         // +0
    char *vtable;                         // +4
};

extern eMaterialFlipObject *D_0037D0E4;

class eMaterial {
public:
    static void Flip(void);
};

void eMaterial::Flip(void) {
    eMaterialFlipObject *p = D_0037D0E4;
    if (p != 0) {
        eMaterialFlipVTableEntry *entry = (eMaterialFlipVTableEntry *)(p->vtable + 0xA8);
        short off = entry->offset;
        void (*fn)(void *) = entry->fn;
        fn((char *)p + off);
    }
    D_0037D0E4 = 0;
}

// ============================================================
// Function 5: eInputMouse::Initialize(void) static @ 0x0003b13c
// ============================================================
extern char D_00045338[];

struct eInputMouseState3 {
    char _pad[0x80];
    int m80;                              // +0x80
    int m84;                              // +0x84
    char _pad2[0x9C - 0x88];
    int m9C;                              // +0x9C
};

class eInputMouse {
public:
    static bool PlatformInitialize(void);
    static bool Initialize(void);
};

bool eInputMouse::Initialize(void) {
    if (PlatformInitialize()) {
        eInputMouseState3 *s = (eInputMouseState3 *)D_00045338;
        s->m9C = 0;
        s->m80 = 0;
        s->m84 = 0;
        return true;
    }
    return false;
}
