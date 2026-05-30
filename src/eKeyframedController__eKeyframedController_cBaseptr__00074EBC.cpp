class cBase;

class ePhysicsController {
public:
    ePhysicsController(cBase *);
};

class eKeyframedController : public ePhysicsController {
public:
    eKeyframedController(cBase *);
};

eKeyframedController::eKeyframedController(cBase *b)
    : ePhysicsController(b)
{
    ((void **)this)[1] = (void *)0x383CA0;
    ((int *)this)[5] = 0;
}
