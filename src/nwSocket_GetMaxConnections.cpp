class nwSocket {
public:
    int pad0;
    int pad4;
    int maxConnections;
    int GetMaxConnections(void) const;
};

int nwSocket::GetMaxConnections(void) const {
    return maxConnections;
}
