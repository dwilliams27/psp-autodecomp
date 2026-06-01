#pragma control sched=2

class eInputJoystick {
public:
    enum eButton { eButton_dummy };
};

class gcValControllerState {
public:
    enum gcState { gcState_dummy };
    static void Evaluate(gcState state, eInputJoystick::eButton button, int controllerId);
    static void EvaluateController(int controllerId, gcState state,
                                   eInputJoystick::eButton button);
    static void EvaluateAll(gcState state, eInputJoystick::eButton button);
};

void gcValControllerState::Evaluate(gcState state, eInputJoystick::eButton button,
                                    int controllerId) {
    if (controllerId >= 0) {
        EvaluateController(controllerId, state, button);
        return;
    }
    EvaluateAll(state, button);
}
