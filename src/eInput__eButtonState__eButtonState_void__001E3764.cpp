// eInput::eButtonState::eButtonState(void)
// Address: 0x001e3764, Size: 20B, Obj: eAll_psp.obj
// Symbol: __0o5GeInputMeButtonStatectv

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

eInput::eButtonState::eButtonState()
{
    a = 0;
    b = 0;
    c = 0;
}
