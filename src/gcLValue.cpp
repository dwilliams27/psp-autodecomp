// ODR-WARNING: this TU intentionally redeclares only the small gcExpression /
// gcValue / gcLValue chain needed for gcLValue::Read. Do not replace this with
// a shared header include unless sibling gcValue/gcLValue matches are checked.
class cFile;
class cMemPool;

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

void cFile_SetCurrentPos(void *, unsigned int);

class gcExpression {
public:
    int Read(cFile &, cMemPool *);
};

class gcValue : public gcExpression {
public:
    int Read(cFile &, cMemPool *);
};

class gcLValue : public gcValue {
public:
    int Read(cFile &, cMemPool *);
};

// gcLValue::Read(cFile &, cMemPool *) @ 0x0011B398
int gcLValue::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->gcValue::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}
