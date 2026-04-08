void cStrCopy(char *, const char *, int);

class nwNetwork {
public:
    static void GetLastErrorString(char *, int);
};

void nwNetwork::GetLastErrorString(char *dest, int size) {
    cStrCopy(dest, (const char *)0x371198, size);
}
