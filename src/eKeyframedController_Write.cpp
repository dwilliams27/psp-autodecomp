// eKeyframedController::Write(cFile &) const — 0x00074db4
//
// Same shape as cFactory::Write (matched 76B exemplar): wraps a cWriteBlock
// around a chain-call to the parent class's Write. ePhysicsController is
// declared as a sibling class (not as a base) so the cast `(const
// ePhysicsController *)this` is a reinterpret without `this` adjustment —
// SNC emits `move a0, s1` straight into the call. The method-call form on
// the cast pointer mangles the jal target as ePhysicsController::Write,
// matching the original symbol after relocation masking.

class cBase;
class cFile;

class ePhysicsController {
public:
    void Write(cFile &) const;
};

class eKeyframedController {
public:
    void Write(cFile &) const;
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

#pragma control sched=1
void eKeyframedController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const ePhysicsController *)this)->Write(file);
    wb.End();
}
