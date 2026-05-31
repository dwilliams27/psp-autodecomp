// gcBipedController::OnActivated (0x0014205c) from gcAll_psp.obj.
// Split-TU: class declared locally. Mirrors gcEntityController::GetType +
// trailing SetPhysicsController/OnActivated forwarding idiom.

class cBase;
class cMemPool;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcEntityController {
public:
    void SetPhysicsController(const cType *);
    void OnActivated(void);
};

class gcBipedController : public gcEntityController {
public:
    void OnActivated(void);
};

extern cBase *gcBipedController_New(cMemPool *, cBase *) __asm__("D_00208DDC");

extern cType *D_000385DC;
extern cType *D_000469D8;
extern cType *D_00046BB4;

void gcBipedController::OnActivated(void) {
    if (D_00046BB4 == 0) {
        if (D_000469D8 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_000469D8 = cType::InitializeType(0, 0, 0x232, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_00046BB4 = cType::InitializeType(0, 0, 0x238, D_000469D8,
                                           &gcBipedController_New, 0, 0, 0);
    }
    ((gcEntityController *)this)->SetPhysicsController(D_00046BB4);
    ((gcEntityController *)this)->OnActivated();
}
