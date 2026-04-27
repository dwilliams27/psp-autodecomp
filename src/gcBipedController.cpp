// gcBipedController::OnDeactivated(void)

class cType;

class gcEntityController {
public:
    void OnDeactivated(void);
    void SetPhysicsController(const cType *);
};

class gcBipedController : public gcEntityController {
public:
    void OnDeactivated(void);
};

void gcBipedController::OnDeactivated(void) {
    gcEntityController::OnDeactivated();
    gcEntityController::SetPhysicsController(0);
}
