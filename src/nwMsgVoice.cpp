// nwMsgVoice — nwAll_psp.obj
//   0x0036b808 GetType(void) const  (76B)

class nwMsg;

struct nwMsgBuffer {
    char _pad[0x4B0];
    int  mOffset;
};

class nwMsgType {
public:
    static nwMsgType *InitializeType(unsigned char, unsigned char, const char *,
                                     nwMsg *(*)(nwMsgBuffer &));
};

class nwMsgVoice {
public:
    static nwMsg *New(nwMsgBuffer &);
    nwMsgType *GetType() const;
};

// -----------------------------------------------------------------------------
// 0x0036b808 — nwMsgVoice::GetType
// -----------------------------------------------------------------------------

static nwMsgType *sVoiceType;

nwMsgType *nwMsgVoice::GetType() const {
    if (!sVoiceType) {
        sVoiceType = nwMsgType::InitializeType(
            3, 0, "nwMsgVoice", &nwMsgVoice::New);
    }
    return sVoiceType;
}
