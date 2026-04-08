class gcDesiredObjectHelper {
public:
    enum gcPrimary {};
    static const char *GetPrimaryText(gcPrimary);
};

const char *gcDesiredObjectHelper::GetPrimaryText(gcPrimary) {
    return (const char *)0x36DAF0;
}

class gcDoSetValue {
public:
    enum gcOp {};
    static const char *GetOpText(gcOp);
};

const char *gcDoSetValue::GetOpText(gcOp) {
    return (const char *)0x36DAF0;
}

class nwNetwork {
public:
    static int GetGameSocket(void);
    static int IsInterfaceStarted(void);
    static int IsInterfaceSecure(void);
};

int nwNetwork::GetGameSocket(void) {
    return *(int *)0x9F930;
}

int nwNetwork::IsInterfaceStarted(void) {
    return *(unsigned char *)0x37D948;
}

int nwNetwork::IsInterfaceSecure(void) {
    return *(unsigned char *)0x37D949;
}
