struct nwMsgVoice;
struct nwSocketHandle { int mValue; };
struct nwConnectionHandle { int mValue; };

class nwHeadset {
public:
    static void SendVoiceData(const nwMsgVoice &, nwSocketHandle, nwConnectionHandle);
};

void nwHeadset::SendVoiceData(const nwMsgVoice &, nwSocketHandle, nwConnectionHandle) {
}
