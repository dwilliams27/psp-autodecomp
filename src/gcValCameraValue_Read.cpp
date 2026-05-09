// ODR-WARNING: split-TU method addition for gcValCameraValue::Read only.
// Keep this local redeclaration narrow to avoid perturbing matched siblings.

class cBase;
class cFile;
class cMemPool;

class gcValCameraValue {
public:
    int Read(cFile &, cMemPool *);
};

int gcValue_Read(gcValCameraValue *, cFile &, cMemPool *);
void cFile_SetCurrentPos(void *, unsigned int);

extern "C" {
    void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
    void __0oKcReadBlockdtv(void *, int);
}

int gcValCameraValue::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];

    __0oKcReadBlockctR6FcFileUib(rb, file, 3, true);
    if ((unsigned int)rb[3] == 3 && gcValue_Read(this, file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
    __0oKcReadBlockdtv(rb, 2);
    return 0;
success:
    __0oKcReadBlockdtv(rb, 2);
    return result;
}
