// gcMsgAckPlayerState::GetType(void) const  @ 0x00285864  (gcAll_psp.obj)
// Emits: __0fTgcMsgAckPlayerStateHGetTypevK

class nwMsgBuffer;
class nwMsg;
class nwMsgType;

class nwMsgType {
public:
    static nwMsgType *InitializeType(unsigned char, unsigned char,
                                     const char *, nwMsg *(*)(nwMsgBuffer &));
};

class gcMsgAckPlayerState {
public:
    static nwMsg *New(nwMsgBuffer &);
    const nwMsgType *GetType(void) const;
};

static nwMsgType *type_gcMsgAckPlayerState asm("D_0009F540");

const nwMsgType *gcMsgAckPlayerState::GetType(void) const {
    if (!type_gcMsgAckPlayerState) {
        type_gcMsgAckPlayerState = nwMsgType::InitializeType(
            8, 1, (const char *)0x36E244, gcMsgAckPlayerState::New);
    }
    return type_gcMsgAckPlayerState;
}
