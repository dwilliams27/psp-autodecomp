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
class eDrawInfo;
class eColor;
class cBase;
class cFile;
class cMemPool;

class eBumpOffsetMap : public eTexture {
public:
    bool field_4C;
    char _pad_4D[3];
    float field_50;
    int field_54;

    eBumpOffsetMap(cBase *);
    void PlatformFree(void);
    void CreateData(void);
    void Apply(int, const eCamera *, const eWorld *) const;
    void Write(cFile &) const;
    int GetNumExternalDependencies(void) const;

    static eBumpOffsetMap *New(cMemPool *, cBase *);
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
    eDynamicLightModelMtl(cBase *);
    void PlatformFree(void);
    void Apply(const eCamera *, const eWorld *) const;
    void ApplyDynamic(const eDrawInfo &, const mOCS &, float, unsigned int, eColor) const;
    void Unapply(void) const;
    void CreateData(void);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static eDynamicLightModelMtl *New(cMemPool *, cBase *);
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

class eDrawInfo;
class eColor;
class cBase;
class cFile;
class cMemPool;

class eReflectionModelMtl {
public:
    eReflectionModelMtl(cBase *);
    void PlatformFree(void);
    void Unapply(void) const;
    void CreateData(void);
    void ApplyDynamic(const eDrawInfo &, const mOCS &, float, unsigned int, eColor) const;
    void PlatformRead(cFile &, cMemPool *);
    void Write(cFile &) const;
    static eReflectionModelMtl *New(cMemPool *, cBase *);
};

class eRefractionModelMtl {
public:
    eRefractionModelMtl(cBase *);
    void PlatformFree(void);
    void Unapply(void) const;
    void CreateData(void);
    void ApplyDynamic(const eDrawInfo &, const mOCS &, float, unsigned int, eColor) const;
    void PlatformRead(cFile &, cMemPool *);
    void Write(cFile &) const;
    static eRefractionModelMtl *New(cMemPool *, cBase *);
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
    eSilhouetteModelMtl(cBase *);
    void PlatformFree(void);
    void CreateData(void);
    void ApplyDynamic(const eDrawInfo &, const mOCS &, float, unsigned int, eColor) const;
    void Unapply(void) const;
    float GetSilhouetteOffset(const eDrawInfo &) const;
    static eSilhouetteModelMtl *New(cMemPool *, cBase *);
};

#endif
