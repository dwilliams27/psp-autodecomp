class cBase;
class cMemPool;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class eSimulatedMotor {
public:
    const cType *GetType(void) const;
};

extern cType *D_000385DC;
extern cType *D_00046BCC;

const cType *eSimulatedMotor::GetType(void) const {
    __asm__ volatile("" ::: "memory");
    if (D_00046BCC == 0) {
        if (D_000385DC == 0) {
            const char *baseName = (const char *)0x36CD74;
            __asm__ volatile("" : "+r"(baseName) :: "memory");
            const char *baseDesc = (const char *)0x36CD7C;
            __asm__ volatile("" : "+r"(baseDesc) :: "memory");
            D_000385DC = cType::InitializeType(baseName, baseDesc, 1, 0, 0, 0, 0, 0);
        }
        D_00046BCC = cType::InitializeType(0, 0, 0x261, D_000385DC, 0, 0, 0, 0);
    }
    return D_00046BCC;
}
