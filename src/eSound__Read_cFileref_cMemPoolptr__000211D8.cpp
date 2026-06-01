// eSound::Read(cFile &, cMemPool *) @ 0x000211d8, 120B  (eAll_psp.obj)
// Isolated split-TU reconstruction emitting __0fGeSoundEReadR6FcFileP6IcMemPool.
//
// Versioned read. A cReadBlock is constructed on the stack from the file
// (version 1, owned=true). Its version field (offset 12 = rb[3]) is compared
// to 1; on mismatch the file is rewound via cFile::SetCurrentPos(mFile,
// mStart) and the function returns 0, otherwise it returns 1.
//
// Mirrors the matched eSoundData::Read structure: the cReadBlock ctor/dtor are
// invoked through their concrete mangled symbols as plain calls over an
// int rb[5] frame slot (rather than an implicit C++ object). This is what
// makes SNC's prologue scheduler defer the `sw ra` save to just before the
// first call (matching the original) and keep the success-path constant 1
// live in the callee-saved s0 across the dtor call.

class cFile;
class cMemPool;

// cReadBlock layout over rb[5]: [0]=mFile, [1]=mStart, [2]=mSize,
// [3]=mEnd/version, [4]=mOwned.
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

// Method call to cFile::SetCurrentPos lowers to jal with (mFile, mStart);
// the relocation target is masked by compare_func.
extern "C" void cFile_SetCurrentPos(void *, unsigned int);

class eSound {
public:
    int Read(cFile &, cMemPool *);
};

// ── eSound::Read(cFile &, cMemPool *) @ 0x000211d8 ──
int eSound::Read(cFile &file, cMemPool *) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 1, true);
    if (rb[3] != 1) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }
    __0oKcReadBlockdtv(rb, 2);
    return result;
}
