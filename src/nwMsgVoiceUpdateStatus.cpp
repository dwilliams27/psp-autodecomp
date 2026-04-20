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

class nwMsgVoiceUpdateStatus {
public:
    static nwMsg *New(nwMsgBuffer &);
    nwMsgType *GetType() const;
};

class nwMsgConnectionAccepted {
public:
    static nwMsg *New(nwMsgBuffer &);
};

extern char nwMsgVoiceUpdateStatusvirtualtable[];
extern char nwMsgConnectionAcceptedvirtualtable[];

static nwMsgType *sVoiceUpdateStatusType;

nwMsg *nwMsgVoiceUpdateStatus::New(nwMsgBuffer &buf) {
    buf.mOffset += 12;
    char *p = (char *)&buf + buf.mOffset;
    nwMsg *result = 0;
    if (p) {
        *(char **)p = nwMsgVoiceUpdateStatusvirtualtable;
        result = (nwMsg *)p;
    }
    return result;
}

nwMsgType *nwMsgVoiceUpdateStatus::GetType() const {
    if (!sVoiceUpdateStatusType) {
        sVoiceUpdateStatusType = nwMsgType::InitializeType(4, 0, "nwMsgVoiceUpdateStatus", &nwMsgVoiceUpdateStatus::New);
    }
    return sVoiceUpdateStatusType;
}

nwMsg *nwMsgConnectionAccepted::New(nwMsgBuffer &buf) {
    buf.mOffset += 4;
    char *p = (char *)&buf + buf.mOffset;
    nwMsg *result = 0;
    if (p) {
        *(char **)p = nwMsgConnectionAcceptedvirtualtable;
        result = (nwMsg *)p;
    }
    return result;
}
