class cFile;
class cMemPool;

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

void cFile_SetCurrentPos(void *, unsigned int);

class gcStringValue {
public:
    int Read(cFile &, cMemPool *);
};

int gcStringValue::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    cReadBlock rb(file, 1, true);
    if (rb._data[3] == 1)
        goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}
