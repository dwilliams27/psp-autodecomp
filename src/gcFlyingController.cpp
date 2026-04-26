// gcFlyingController — methods from gcAll_psp.obj.

class cTimeValue {
public:
    int mTime;
};

class gcEntityController {
public:
    void Update(cTimeValue);
};

class gcFlyingController : public gcEntityController {
public:
    void Update(cTimeValue);
};

// ── Update (0x00153f14) ──

void gcFlyingController::Update(cTimeValue t) {
    gcEntityController::Update(t);
}
