class nwNetwork {
public:
    static int GetGameVersion(void);
};

int nwNetwork::GetGameVersion(void) {
    return (*(int **)0x37D94C)[2];
}
