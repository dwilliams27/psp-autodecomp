class cMemPool;

class gcUIGeomTemplate {
public:
    void Reset(cMemPool *, bool);
};

class gcPartialBodyController {
public:
    void OnActivated(void);
};

class gcSubGeomController {
public:
    void Reset(cMemPool *, bool);
};

enum nwTransportType {};

class nwTransport {
public:
    static nwTransport *Create(nwTransportType, unsigned short);
};

class nwNetwork {
public:
    static void *GetLobby(void);
};

void gcUIGeomTemplate::Reset(cMemPool *, bool) {
}

void gcPartialBodyController::OnActivated(void) {
}

void gcSubGeomController::Reset(cMemPool *, bool) {
}

nwTransport *nwTransport::Create(nwTransportType, unsigned short) {
    return 0;
}

void *nwNetwork::GetLobby(void) {
    return 0;
}
