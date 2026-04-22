class nwMsg;
class cOutStream;
class cInStream;
class nwAddress;
typedef int nwSocketHandle;
typedef int nwConnectionHandle;

struct nwMsgBuffer {
    char _pad[0x4B0];
    int mOffset;
};

class nwMsgType {
public:
    static nwMsgType *InitializeType(unsigned char, unsigned char, const char *, nwMsg *(*)(nwMsgBuffer &));
};

class cOutStreamRef {
public:
    void Write(int, int, bool);
};

class cInStreamRef {
public:
    void Read(int &, int, bool);
};

class gcMsgConnected {
public:
    static nwMsg *New(nwMsgBuffer &);
    nwMsgType *GetType() const;
    void Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const;
    void Read(cInStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle);

    void *mVTable;
    signed char mState;
};

class gcMsgAckPlayerState {
public:
    static nwMsg *New(nwMsgBuffer &);
};

extern char gcMsgConnectedvirtualtable[];
extern char gcMsgAckPlayerStatevirtualtable[];
extern int gConnectedState;

static nwMsgType *sConnectedType;

nwMsg *gcMsgConnected::New(nwMsgBuffer &buf) {
    buf.mOffset += 8;
    char *p = (char *)&buf + buf.mOffset;
    nwMsg *result = 0;
    if (p) {
        *(char **)p = gcMsgConnectedvirtualtable;
        *(char *)(p + 4) = 0;
        result = (nwMsg *)p;
    }
    return result;
}

nwMsgType *gcMsgConnected::GetType() const {
    if (!sConnectedType) {
        sConnectedType = nwMsgType::InitializeType(0x13, 0, "gcMsgConnected", &gcMsgConnected::New);
    }
    return sConnectedType;
}

void gcMsgConnected::Write(cOutStream &s, nwSocketHandle, const nwAddress &, nwConnectionHandle) const {
    signed char v = mState;
    int iv = v;
    ((cOutStreamRef *)&s)->Write((signed char)iv, 8, true);
}

void gcMsgConnected::Read(cInStream &s, nwSocketHandle sock, const nwAddress &, nwConnectionHandle conn) {
    volatile nwSocketHandle vol_sock = sock;
    volatile nwConnectionHandle vol_conn = conn;
    int value;
    (void)vol_sock; (void)vol_conn;
    ((cInStreamRef *)&s)->Read(value, 8, true);
    mState = (char)value;
    gConnectedState = mState;
}

nwMsg *gcMsgAckPlayerState::New(nwMsgBuffer &buf) {
    buf.mOffset += 8;
    char *p = (char *)&buf + buf.mOffset;
    nwMsg *result = 0;
    if (p) {
        *(char **)p = gcMsgAckPlayerStatevirtualtable;
        *(int *)(p + 4) = 0;
        result = (nwMsg *)p;
    }
    return result;
}

// =============================================================
// Function 3: gcDoSetValue::GetText(char *) const @ 0x0014f868
// =============================================================

extern "C" void cStrCat(char *, const char *);

class gcDoSetValue {
public:
    enum gcOp { };
    void GetText(char *) const;
    static const char *GetOpText(gcOp);
    char _pad[0xC];
    gcOp mOp;
};

void gcDoSetValue::GetText(char *buf) const {
    cStrCat(buf, GetOpText(mOp));
}

// =============================================================
// Function 4: gcDoSwitchCase::GetTextRecursive(char *) const @ 0x00150038
// =============================================================

class gcAction {
public:
    void GetTextRecursive(char *) const;
};

class gcDoSwitchCase : public gcAction {
public:
    void GetTextRecursive(char *) const;
};

extern const char gcDoSwitchCase_text_suffix[];

void gcDoSwitchCase::GetTextRecursive(char *buf) const {
    gcAction::GetTextRecursive(buf);
    cStrCat(buf, gcDoSwitchCase_text_suffix);
}

// =============================================================
// Function 5: gcTableColumnStringBuffer::Set(int, const wchar_t *, bool) @ 0x00272d94
// =============================================================

extern "C" void cStrCopy(wchar_t *, const wchar_t *, int);

class gcTableColumnStringBuffer {
public:
    void Set(int, const wchar_t *, bool);
    char _pad[8];
    int mMaxLen;        // 0x8
    wchar_t **mStrings; // 0xC
};

void gcTableColumnStringBuffer::Set(int index, const wchar_t *src, bool) {
    cStrCopy(mStrings[index], src, mMaxLen);
}
