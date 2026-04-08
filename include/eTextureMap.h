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
    void CreateData(void);
};

class eCamera;
class eWorld;
class mOCS;
class mVec3;
class eTexture;

class eBumpOffsetMap {
public:
    void PlatformFree(void);
    void CreateData(void);
    void Apply(int, const eCamera *, const eWorld *) const;
};

class eDynamicLightHeightmapMtl {
public:
    void PlatformFree(void);
    void Apply(const eCamera *, const eWorld *) const;
    void ApplyDynamic(const eCamera &, const eWorld *, const mOCS &, float, float, const mVec3 &, float, int, const eTexture *, unsigned int) const;
    void Unapply(void) const;
    void CreateData(void);
};

class eDynamicLightModelMtl {
public:
    void PlatformFree(void);
    void Apply(const eCamera *, const eWorld *) const;
    void Unapply(void) const;
    void CreateData(void);
};

class eOnePassModelMtl {
public:
    void PlatformFree(void);
    void Unapply(void) const;
};

#endif
