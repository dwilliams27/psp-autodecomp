// eInputJoystick::eInputState::eInputState(void)
// Address: 0x001e832c, Size: 76B, Obj: eAll_psp.obj
// Symbol: __0o5OeInputJoystickLeInputStatectv

#pragma control sched=2

// Embedded element type — array member needs a non-trivial ctor so the
// compiler emits a __vec_new call.
class eInput {
public:
    class eButtonState {
    public:
        char pad0[12];  // 0..11 (not initialized by ctor)
        int  a;         // 12
        int  b;         // 16
        int  c;         // 20
        eButtonState();
    };
};

class eInputJoystick {
public:
    struct eInputState {
        eInput::eButtonState buttons[86];   // 0x000..0x810
        char gap0[8];                       // 0x810..0x818
        int  f818;                          // 0x818
        int  f81C;                          // 0x81C
        char gap1[4];                       // 0x820..0x824
        int  f824;                          // 0x824
        char gap2[8];                       // 0x828..0x830
        int  f830;                          // 0x830
        char gap3[12];                      // 0x834..0x840
        int  f840;                          // 0x840
        eInputState();
    };
};

eInputJoystick::eInputState::eInputState()
{
    f818 = 0;
    f81C = 0;
    f824 = 0;
    f830 = 0;
    f840 = 0;
}
