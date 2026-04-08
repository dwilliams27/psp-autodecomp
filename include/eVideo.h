#ifndef EVIDEO_H
#define EVIDEO_H

class eCamera;
class eVertexShader;

class eVideo {
public:
    static void PreFlip(void);
    static void EndStencil(void);
    static void ApplyVertexShader(const eCamera *, const eVertexShader *);
    static void SetAlphaWriteEnable(bool);
    static void SetFlippedBFCMode(bool);
    static int IsWidescreen(void);
};

#endif
