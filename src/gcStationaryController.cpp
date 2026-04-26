// gcStationaryController — methods from gcAll_psp.obj.

class cTimeValue {
public:
    int mTime;
};

class gcEntityController {
public:
    void HandleNextAnimation(cTimeValue);
};

class gcStationaryController : public gcEntityController {
public:
    void Move(cTimeValue);
};

// ── Move (0x00158504) ──

void gcStationaryController::Move(cTimeValue t) {
    HandleNextAnimation(t);
}
