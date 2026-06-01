// gcMsgEntitySendMessage::WriteEventData — gcAll_psp.obj
//   0x001341bc  WriteEventData(cOutStream &, ...) static  (124B)
//
// Split-TU: the class is declared locally so this is the only file emitting
// the WriteEventData symbol (the shared TU only declares + calls it).

class cOutStream;
class gcEnumeration;
class gcEnumerationEntry;
class gcEventParams;
class gcEventStackData;
template <class A, class B> class cHandlePairT;
template <class A> class cSubHandleT;

class cOutStreamRef {
public:
    void Write(unsigned int, int, bool);
};

class cHandle {
public:
    void Write(cOutStream &) const;
};

class gcEventStackData {
public:
    void Write(cOutStream &) const;
};

class gcEventParams {
public:
    void Write(cOutStream &) const;
};

class gcMsgEntitySendMessage {
public:
    static void WriteEventData(cOutStream &,
                               const cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > &,
                               const gcEventParams &,
                               const gcEventStackData &);
};

void gcMsgEntitySendMessage::WriteEventData(
        cOutStream &s,
        const cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > &handle,
        const gcEventParams &params,
        const gcEventStackData &stack) {
    ((const cHandle *)&handle)->Write(s);
    ((cOutStreamRef *)&s)->Write(*(const unsigned int *)((const char *)&handle + 4), 0x20, true);
    stack.Write(s);
    params.Write(s);
}
