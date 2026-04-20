#include "eRenderSurface.h"

// ── eVRAMMgr allocator interface ──

class eVRAMMgr {
public:
    static int AllocatePages(int);
    static void DeallocatePages(int);
};

// ── cMemPool interface ──

class cMemPool {
public:
    static void *GetPoolFromPtr(const void *);
};

extern "C" void free(void *);

// Global head of the linked list of render surfaces.
extern eRenderSurface *gRenderSurfaceHead;

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

// ── Constructor ──

eRenderSurface::eRenderSurface(void) {
    field_0 = 0;
    __asm__ volatile("" ::: "memory");
    mVRAMPage = -1;
    mSizeBytes = 0;
    mWidth = 0;
    mHeight = 0;
    mBytesPerPixel = 0;
    mPixelFormat = 3;
    mPrev = 0;
    mNext = 0;
}

// ── Uninitialize ──

void eRenderSurface::Uninitialize(void) {
    __asm__ volatile("" ::: "memory");
    if (mVRAMPage >= 0) {
        eVRAMMgr::DeallocatePages(mVRAMPage);
        __asm__ volatile("" ::: "memory");
        mVRAMPage = -1;
        mSizeBytes = 0;
    }
    eRenderSurface *self = this;
    if (self != 0 && self->mPrev != 0 && self->mNext != 0) {
        if (gRenderSurfaceHead == self) {
            gRenderSurfaceHead = self->mNext;
        }
        self->mPrev->mNext = self->mNext;
        self->mNext->mPrev = self->mPrev;
        self->mPrev = 0;
        self->mNext = 0;
        if (gRenderSurfaceHead == self) {
            gRenderSurfaceHead = 0;
        }
    }
}

// ── Destructor ──

extern "C" {

void eRenderSurface___dtor_eRenderSurface_void(eRenderSurface *self, int flags) {
    if (self != 0) {
        self->Uninitialize();
        if (flags & 1) {
            void *pool = cMemPool::GetPoolFromPtr(self);
            if (pool != 0) {
                void *block = *(void **)((char *)pool + 0x24);
                __asm__ volatile("" ::: "memory");
                DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
                short off = rec->offset;
                rec->fn((char *)block + off, self);
            } else {
                free(self);
            }
        }
    }
}

}

// ── Initialize ──

void eRenderSurface::Initialize(int width, int height, unsigned int fmt, bool hiColor, int pages) {
    Uninitialize();
    field_0 = fmt;
    mSizeBytes = 0;
    mWidth = (short)width;
    mHeight = (short)height;
    mBytesPerPixel = hiColor ? 4 : 2;
    mPixelFormat = hiColor ? 3 : 1;
    __asm__ volatile("" ::: "memory");
    unsigned int rounded = (((unsigned int)(width + 63)) >> 6) << 6;
    int prod = rounded * height;
    __asm__ volatile("" : "+r"(prod) ::);
    mSizeBytes = prod * mBytesPerPixel;
    __asm__ volatile("" ::: "memory");

    int page = pages;
    if (pages <= 0) {
        page = eVRAMMgr::AllocatePages(mSizeBytes);
    }
    mVRAMPage = page;

    eRenderSurface *head = gRenderSurfaceHead;
    if (head != 0) {
        mPrev = head->mPrev;
        mNext = gRenderSurfaceHead;
        mPrev->mNext = this;
        mNext->mPrev = this;
    } else {
        gRenderSurfaceHead = this;
        mNext = this;
        mPrev = this;
    }
}

// ── gcString constructor ──

class cBase;

class gcString {
public:
    cBase *mParent;
    void *mVTable;
    int mField_08;
    int mField_0C;
    int mField_10;
    int mField_14;
    int mField_18;
    int mField_1C;
    int mField_20;

    gcString(cBase *);
};

extern char gcEventvirtualtable[];

gcString::gcString(cBase *parent) {
    mParent = parent;
    mVTable = gcEventvirtualtable;
    mField_08 = 0;
    mField_0C = 0;
    mField_10 = 0;
    mField_14 = 0;
    mField_18 = 0;
    mField_1C = 0;
    mField_20 = 0;
}
