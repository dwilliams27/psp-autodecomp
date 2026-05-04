// gcUIStackDialog::~gcUIStackDialog(void) @ 0x00109614 (180B, gcAll_psp.obj)
//
// Destructor: closes the live dialog (if any) via the global gcUI singleton
// at 0x99928, optionally flushes video, then SNC emits the standard deleting
// tail (pool-aware operator delete with free() fallback).

class gcUIDialog;
class gcDoUIShowDialog;

class gcUI {
public:
    void CloseDialog(gcUIDialog *, const gcDoUIShowDialog *, float *);
};

class eVideo {
public:
    static void Flush();
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

extern "C" void free(void *);

struct DelRec_gcUIStackDialog {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcUIStackDialog {
public:
    gcUIDialog *mDialog;       // 0x00
    unsigned char mDrawTwice;  // 0x04

    ~gcUIStackDialog();

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        if (pool != 0) {
            void *block = *(void **)((char *)pool + 0x24);
            DelRec_gcUIStackDialog *rec =
                (DelRec_gcUIStackDialog *)(*(char **)((char *)block + 0x1C) +
                                           0x30);
            short off = rec->offset;
            void (*fn)(void *, void *) = rec->fn;
            fn((char *)block + off, p);
        } else {
            free(p);
        }
    }
};

gcUIStackDialog::~gcUIStackDialog() {
    if (mDialog != 0) {
        ((gcUI *)0x99928)->CloseDialog(mDialog, 0, 0);
    }
    if (mDrawTwice != 0) {
        eVideo::Flush();
    }
}
