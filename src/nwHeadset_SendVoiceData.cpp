struct nwMsgVoice;
typedef int nwSocketHandle;
typedef int nwConnectionHandle;

class nwHeadset {
public:
    static void SendVoiceData(const nwMsgVoice &, nwSocketHandle, nwConnectionHandle);
};

void nwHeadset::SendVoiceData(const nwMsgVoice &, nwSocketHandle, nwConnectionHandle) {
}
