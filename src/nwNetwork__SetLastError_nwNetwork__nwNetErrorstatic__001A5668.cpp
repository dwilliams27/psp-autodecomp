class nwNetwork {
public:
    enum nwNetError {};
    static void SetLastError(nwNetError);
};

typedef void (*nwNetErrorCallback)();

void nwNetwork::SetLastError(nwNetError err) {
    int *base = (int *)0x380000;
    if (base[-2473] != (int)err) {
        base[-2473] = (int)err;
        if (err != 0) {
            nwNetErrorCallback cb = *(nwNetErrorCallback *)0x37D954;
            if (cb != 0) {
                cb();
            }
        }
    }
}
