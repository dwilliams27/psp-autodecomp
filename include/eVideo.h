#ifndef EVIDEO_H
#define EVIDEO_H

class eCamera;
class eVertexShader;

class eVideo {
public:
    enum eVideoMode {};

    static void SetVideoMode(eVideoMode);
    static void PreFlip(void);
    static void EndStencil(void);
    static void ApplyVertexShader(const eCamera *, const eVertexShader *);
    static void SetAlphaWriteEnable(bool);
    static void SetFlippedBFCMode(bool);
    static int IsWidescreen(void);
    static struct eViewport *GetFullscreenViewport(void);
};

#endif
