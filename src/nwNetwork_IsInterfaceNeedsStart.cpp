class nwNetwork {
public:
    static int IsInterfaceNeedsStart(void);
};

int nwNetwork::IsInterfaceNeedsStart(void) {
    return *(unsigned char *)0x37D948 == 0;
}
