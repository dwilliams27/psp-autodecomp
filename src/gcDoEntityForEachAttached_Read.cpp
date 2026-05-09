#include "gcDoEntityForEach.h"
#include "cBase.h"

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class gcExpressionList {
public:
    int Read(cReadBlock &);
};

class cFile {
public:
    void SetCurrentPos(unsigned int);
};

int gcDoEntityForEachAttached::Read(cFile &file, cMemPool *pool) {
    register int ok __asm__("$19");
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(ok));
    if ((unsigned int)rb._data[3] != 1 || !gcDoEntityFindAttachedBase::Read(file, pool)) {
        ((cFile *)rb._data[0])->SetCurrentPos((unsigned int)rb._data[1]);
        return 0;
    }
    ((gcExpressionList *)((char *)this + 100))->Read(rb);
    return ok;
}
