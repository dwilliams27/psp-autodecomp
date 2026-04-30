void cStrCopy(char *, const char *, int);

class nwNetwork {
public:
    enum nwNetError {};
    static int GetLastPlatformError(void);
    static void GetInterfaceName(int, char *, int);
    static void CheckSecurity(void);
    static void StopInterface(void);
    static int GetSelectedInterface(void);
    static void StartInterface(void);
    static int GetNumInterfaces(void);
    static int IsInterfaceValid(int);
    static void SetLastError(nwNetError);
};

int nwNetwork::GetLastPlatformError(void) {
    return 0;
}

void nwNetwork::GetInterfaceName(int, char *dest, int size) {
    cStrCopy(dest, (const char *)0x371198, size);
}

void nwNetwork::CheckSecurity(void) {
    SetLastError(nwNetError(0));
    *(unsigned char *)0x37D949 = 1;
}

void nwNetwork::StopInterface(void) {
    SetLastError(nwNetError(0));
    *(unsigned char *)0x37D948 = 0;
    *(unsigned char *)0x37D949 = 0;
}

void nwNetwork::StartInterface(void) {
    SetLastError(nwNetError(0));
    *(unsigned char *)0x37D948 = 1;
    *(unsigned char *)0x37D949 = 0;
}

int nwNetwork::GetSelectedInterface(void) {
    int selected = -1;
    if (GetNumInterfaces() > 0) {
        selected = 0;
    }
    return selected;
}

int nwNetwork::IsInterfaceValid(int index) {
    int valid = 0;
    if (index >= 0 && index < GetNumInterfaces()) {
        valid = 1;
    }
    return valid & 0xff;
}
