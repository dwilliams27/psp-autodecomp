#ifndef ERENDERSURFACE_H
#define ERENDERSURFACE_H

class eRenderSurface {
public:
    unsigned int    field_0;
    int             mVRAMPage;
    int             mSizeBytes;
    short           mWidth;
    short           mHeight;
    unsigned short  mBytesPerPixel;
    short           mPixelFormat;
    eRenderSurface *mPrev;
    eRenderSurface *mNext;

    eRenderSurface(void);
    ~eRenderSurface(void);
    void Initialize(int, int, unsigned int, bool, int);
    void Uninitialize(void);
};

#endif
