#ifndef ERENDERTARGET_H
#define ERENDERTARGET_H

class eRenderTarget {
public:
    static int Initialize(void);
    static void Uninitialize(void);
    static void Flip(void);
};

#endif
