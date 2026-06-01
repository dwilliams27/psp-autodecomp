#pragma control sched=2

class eInputJoystick {
public:
    enum eButton { eButton_dummy };
};

class gcValControllerState {
public:
    enum gcState { gcState_dummy };
    static float EvaluateController(int controllerId, gcState state,
                                    eInputJoystick::eButton button);
    static float EvaluateAll(gcState state, eInputJoystick::eButton button);
};

float gcValControllerState::EvaluateAll(gcState state, eInputJoystick::eButton button) {
    if (state == (gcState)7) {
        return EvaluateController(-1, state, button);
    }
    float total = 0.0f;
    int i = 0;
    do {
        total += EvaluateController(i, state, button);
        i++;
    } while (i < 4);
    return total;
}
