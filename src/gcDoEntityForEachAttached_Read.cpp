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
    int ok = 1;
    cReadBlock rb(file, 1, true);
    if (rb._data[3] != ok || !gcDoEntityFindAttachedBase::Read(file, pool)) {
        ((cFile *)rb._data[0])->SetCurrentPos((unsigned int)rb._data[1]);
        return 0;
    }
    ((gcExpressionList *)((char *)this + 100))->Read(rb);
    return ok;
}
