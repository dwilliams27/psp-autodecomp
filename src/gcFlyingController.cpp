// gcFlyingController — methods from gcAll_psp.obj.

class cTimeValue {
public:
    int mTime;
};

class gcEntityController {
public:
    void Update(cTimeValue);
    void OnSelected();
    void OnDeselected();
};

class gcFlyingController : public gcEntityController {
public:
    void Update(cTimeValue);
    void OnSelected();
    void OnDeselected();
};

// ── Update (0x00153f14) ──

void gcFlyingController::Update(cTimeValue t) {
    gcEntityController::Update(t);
}

// ── OnSelected (0x00153f30) ──

void gcFlyingController::OnSelected() {
    gcEntityController::OnSelected();
}

// ── OnDeselected (0x00153f4c) ──

void gcFlyingController::OnDeselected() {
    gcEntityController::OnDeselected();
}
