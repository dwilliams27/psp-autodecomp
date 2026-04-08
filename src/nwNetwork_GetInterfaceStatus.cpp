void cStrCopy(char *, const char *, int);

class nwNetwork {
public:
    static void GetInterfaceStatus(char *, int);
};

void nwNetwork::GetInterfaceStatus(char *dest, int size) {
    cStrCopy(dest, (const char *)0x371198, size);
}
