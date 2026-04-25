// gcMsgCheckSynchronization — gcAll_psp.obj
// Also houses matches for gcString::GetName and
// gcTableColumnStringTable::AssignCopy in the same TU.

class nwMsg;
class cOutStream;
class cInStream;
class nwAddress;
class cBase;

class nwSocketHandle {
public:
    int mHandle;
};

class nwConnectionHandle {
public:
    int mHandle;
};

struct nwMsgBuffer {
    char _pad[0x4B0];
    int mOffset;
};

class nwMsgType {
public:
    static nwMsgType *InitializeType(unsigned char, unsigned char, const char *,
                                     nwMsg *(*)(nwMsgBuffer &));
};

class cOutStreamRef {
public:
    void Write(unsigned int, int, bool);
};

class cInStreamRef {
public:
    void Read(unsigned int &, int, bool);
};

extern "C" int cRand(void);

// -----------------------------------------------------------------------------
// gcMsgCheckSynchronization
// -----------------------------------------------------------------------------

class gcMsgCheckSynchronization {
public:
    static nwMsg *New(nwMsgBuffer &);
    nwMsgType *GetType(void) const;
    void Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const;
    void Read(cInStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle);
};

static nwMsgType *sCheckSyncType;

extern char gcMsgCheckSynchronizationvirtualtable[];

// 0x00285b48, 48B
nwMsg *gcMsgCheckSynchronization::New(nwMsgBuffer &buf) {
    buf.mOffset += 4;
    char *p = (char *)&buf + buf.mOffset;
    nwMsg *result = 0;
    if (p) {
        *(char **)p = gcMsgCheckSynchronizationvirtualtable;
        result = (nwMsg *)p;
    }
    return result;
}

// 0x00136c0c, 56B
void gcMsgCheckSynchronization::Write(cOutStream &s, nwSocketHandle, const nwAddress &, nwConnectionHandle) const {
    ((cOutStreamRef *)&s)->Write((unsigned int)cRand(), 0x20, true);
}

// 0x00136c44, 56B
void gcMsgCheckSynchronization::Read(cInStream &s, nwSocketHandle sock, const nwAddress &, nwConnectionHandle conn) {
    volatile nwSocketHandle vol_sock;
    volatile nwConnectionHandle vol_conn;
    unsigned int tmp;
    vol_sock.mHandle = sock.mHandle;
    vol_conn.mHandle = conn.mHandle;
    ((cInStreamRef *)&s)->Read(tmp, 0x20, true);
    cRand();
}

// 0x00285afc, 76B
nwMsgType *gcMsgCheckSynchronization::GetType(void) const {
    if (!sCheckSyncType) {
        sCheckSyncType = nwMsgType::InitializeType(
            0x23, 0, "gcMsgCheckSynchronization",
            &gcMsgCheckSynchronization::New);
    }
    return sCheckSyncType;
}

// -----------------------------------------------------------------------------
// gcString::GetName  @ 0x0023aa48 (gcAll_psp.obj, 56B)
// -----------------------------------------------------------------------------

void cStrCopy(char *, const wchar_t *, int);

class gcString {
public:
    char _pad[24];
    const wchar_t *mName;   // offset 24
    void GetName(char *) const;
};

void gcString::GetName(char *dest) const {
    const wchar_t *name;
    if (mName != 0) {
        name = mName;
    } else {
        name = (const wchar_t *)0x36daf4;
    }
    cStrCopy(dest, name, 0x50);
}

// -----------------------------------------------------------------------------
// gcTableColumnStringTable::AssignCopy  @ 0x00272140 (gcAll_psp.obj, 56B)
// -----------------------------------------------------------------------------

struct StringTableRef {
    int mIndex;
};

class gcTableColumnStringTable {
public:
    char _pad[8];
    StringTableRef mTable;   // offset 8
    void AssignCopy(const cBase *);
};

gcTableColumnStringTable *dcast(const cBase *);

void gcTableColumnStringTable::AssignCopy(const cBase *base) {
    gcTableColumnStringTable *other = dcast(base);
    mTable = other->mTable;
}
