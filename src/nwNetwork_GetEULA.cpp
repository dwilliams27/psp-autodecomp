void cStrCopy(char *, const char *, int);

class nwNetwork {
public:
    static void GetEULA(char *, int);
};

void nwNetwork::GetEULA(char *dest, int size) {
    cStrCopy(dest, (const char *)0x371198, size);
}
