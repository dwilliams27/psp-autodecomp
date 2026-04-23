#include "eSimulatedController.h"
#include "cFile.h"

#pragma control sched=1

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class ePhysicsController {
public:
    void Write(cFile &) const;
};

void eSimulatedController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const ePhysicsController *)this)->Write(file);
    wb.End();
}
