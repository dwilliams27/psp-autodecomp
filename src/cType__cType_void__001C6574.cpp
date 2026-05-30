// cType::cType(void) @ 0x001c6574, 20B, cAll_psp.obj
// __0oFcTypectv

extern int g_cType_target;

class cType {
public:
    char pad[0x2C];
    void *m2C;
    cType();
};

cType::cType() {
    m2C = &g_cType_target;
}
