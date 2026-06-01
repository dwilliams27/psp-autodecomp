// ODR-WARNING: split-TU isolated reconstruction of gcMsgEntitySendMessage::ReadEventData.
// 0x00134238 gcMsgEntitySendMessage::ReadEventData(cInStream &,
//   cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > &,
//   gcEventParams &, gcEventStackData &) static (124B)
// Class/types declared locally to avoid touching shared TUs / headers.

class cInStream;
class gcEnumeration;
class gcEnumerationEntry;

template <class T> class cSubHandleT {
public:
    int mValue;
};

template <class A, class B> class cHandlePairT {
public:
    int  mHandle;      // 0x0
    int  mPad[8];      // accessed at +4 by the stream read
    void Read(cInStream &);
};

class gcEventParams {
public:
    void Read(cInStream &);
};

class gcEventStackData {
public:
    void Read(cInStream &);
};

class gcInStreamReader {
public:
    void Read(unsigned int &, int, bool);
};

class gcMsgEntitySendMessage {
public:
    static void ReadEventData(cInStream &,
                              cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > &,
                              gcEventParams &,
                              gcEventStackData &);
};

void gcMsgEntitySendMessage::ReadEventData(
        cInStream &stream,
        cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > &handle,
        gcEventParams &params,
        gcEventStackData &stackData) {
    handle.Read(stream);
    ((gcInStreamReader *)&stream)->Read(*(unsigned int *)((char *)&handle + 4), 0x20, true);
    stackData.Read(stream);
    params.Read(stream);
}
