#ifndef ETEXTUREMAP_H
#define ETEXTUREMAP_H

class eTextureMap {
public:
    void CreateData(void);
};

class eProjectedModelMtl {
public:
    void PlatformFree(void);
    void Unapply(void) const;
    void CreateData(void);
};

class eShadowFillModelMtl {
public:
    void PlatformFree(void);
};

#endif
