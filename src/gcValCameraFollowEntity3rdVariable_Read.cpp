// ODR-WARNING: split-TU local redeclaration for one method only. Do not add
// this method to the canonical header just to satisfy this translation unit.

class cFile;
class cMemPool;

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

void cFile_SetCurrentPos(void *, unsigned int);
int gcLValue_Read(void *, cFile &, cMemPool *);

class gcValCameraFollowEntity3rdVariable {
public:
    int Read(cFile &, cMemPool *);
};

int gcValCameraFollowEntity3rdVariable::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19");
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 2, true);
    if ((unsigned int)rb._data[3] == 2 && gcLValue_Read(this, file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}
