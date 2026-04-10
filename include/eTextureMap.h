#ifndef ETEXTUREMAP_H
#define ETEXTUREMAP_H

#include "eRenderTarget.h"

class eTextureMap : public eTexture {
public:
    bool field_4C;
    unsigned char field_4D;
    char _pad_4E[2];
    int field_50;

    int GetNumExternalDependencies(void) const;
    eTextureMap(cBase *);
    void Write(cFile &) const;
    eTextureMap &operator=(const eTextureMap &);
    void CreateData(void);

    static eTextureMap *New(cMemPool *, cBase *);
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
    void CreateData(void);
};

class eProjectedDynamicMtl {
public:
    void PlatformFree(void);
    void Unapply(void) const;
    void CreateData(void);
};

class eProjectedHeightmapMtl {
public:
    void PlatformFree(void);
    void Apply(const eCamera *, const eWorld *) const;
    void ApplyDynamic(const eCamera &, const eWorld *, const mOCS &, float, float, const mVec3 &, float, int, const eTexture *, unsigned int) const;
    void Unapply(void) const;
    void CreateData(void);
};

class eReflectionModelMtl {
public:
    void PlatformFree(void);
    void Unapply(void) const;
    void CreateData(void);
};

class eRefractionModelMtl {
public:
    void PlatformFree(void);
    void Unapply(void) const;
    void CreateData(void);
};

class eShadowVolumeModelMtl {
public:
    void PlatformFree(void);
    void CreateData(void);
};

class eExtrudedShadowVolumeModelMtl {
public:
    void CreateData(void);
};

class eSilhouetteModelMtl {
public:
    void PlatformFree(void);
    void CreateData(void);
};

#endif
