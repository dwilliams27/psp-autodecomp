// ODR-WARNING: split TU for ePhysicsController::Read. Keep local declarations
// minimal so this Read match does not perturb the existing ePhysicsController.cpp
// translation unit. Mirrors the matched ePath::Read cReadBlock RAII shape.

class cFile;
class cMemPool;

class cReadBlock {
public:
    int _data[5];
};

extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);
extern "C" void __0fFcFileNSetCurrentPosUi(void *, unsigned int);

class ePhysicsController {
public:
    int Read(cFile &, cMemPool *);
};

#pragma control sched=2
int ePhysicsController::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    cReadBlock rb;

    __0oKcReadBlockctR6FcFileUib(rb._data, file, 1, true);
    if ((unsigned int)rb._data[3] != 1) {
        __0fFcFileNSetCurrentPosUi(*(void **)&rb._data[0], rb._data[1]);
        __0oKcReadBlockdtv(rb._data, 2);
        return 0;
    }

    __0oKcReadBlockdtv(rb._data, 2);
    return result;
}
#pragma control sched=2
