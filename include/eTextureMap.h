#ifndef ETEXTUREMAP_H
#define ETEXTUREMAP_H

#include "eRenderTarget.h"

class cMemPool {
public:
    static float GetSizeScale(void);
    static cMemPool *GetPoolFromPtr(const void *);
};

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
    eProjectedModelMtl(cBase *);
    const cType *GetType(void) const;
    void PlatformFree(void);
    void Unapply(void) const;
    void CreateData(void);
    void PlatformRead(cFile &, cMemPool *);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static eProjectedModelMtl *New(cMemPool *, cBase *);
};

class eCamera;
class eWorld;
class mOCS;
class mVec3;
class eTexture;
class cType;
class eDrawInfo;
class eColor;
class cBase;
class cFile;
class cFilename;
class cMemPool;

class eShadowFillModelMtl {
public:
    eShadowFillModelMtl(cBase *);
    void PlatformFree(void);
    void Unapply(void) const;
    void CreateData(void);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    int Read(cFile &, cMemPool *);
    void PlatformRead(cFile &, cMemPool *);
    void ApplyDynamic(const eDrawInfo &, const mOCS &, float, unsigned int, eColor) const;
    static eShadowFillModelMtl *New(cMemPool *, cBase *);
};

class eBumpOffsetMap : public eTexture {
public:
    bool field_4C;
    char _pad_4D[3];
    float field_50;
    int field_54;

    eBumpOffsetMap(cBase *);
    ~eBumpOffsetMap(void);
    const cType *GetType(void) const;
    void PlatformFree(void);
    void CreateData(void);
    void Apply(int, const eCamera *, const eWorld *) const;
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    int GetNumExternalDependencies(void) const;
    void GetExternalDependency(int, cFilename *) const;

    static void operator delete(void *p) {
        struct DeleteRecord {
            short offset;
            short pad;
            void (*fn)(void *, void *);
        };
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        char *allocTable = ((char **)block)[7];
        DeleteRecord *rec = (DeleteRecord *)(allocTable + 0x30);
        short off = rec->offset;
        __asm__ volatile("" ::: "memory");
        void *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }

    static eBumpOffsetMap *New(cMemPool *, cBase *);
};

class eDynamicLightHeightmapMtl {
public:
    eDynamicLightHeightmapMtl(cBase *);
    const cType *GetType(void) const;
    void PlatformFree(void);
    void Apply(const eCamera *, const eWorld *) const;
    void ApplyDynamic(const eCamera &, const eWorld *, const mOCS &, float, float, const mVec3 &, float, int, const eTexture *, unsigned int) const;
    void Unapply(void) const;
    void CreateData(void);
    void PlatformRead(cFile &, cMemPool *);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static eDynamicLightHeightmapMtl *New(cMemPool *, cBase *);
};

class eDynamicLightModelMtl {
public:
    eDynamicLightModelMtl(cBase *);
    const cType *GetType(void) const;
    void PlatformFree(void);
    void Apply(const eCamera *, const eWorld *) const;
    void ApplyDynamic(const eDrawInfo &, const mOCS &, float, unsigned int, eColor) const;
    void Unapply(void) const;
    void CreateData(void);
    void PlatformRead(cFile &, cMemPool *);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static eDynamicLightModelMtl *New(cMemPool *, cBase *);
};

class eOnePassModelMtl {
public:
    const cType *GetType(void) const;
    eOnePassModelMtl &operator=(const eOnePassModelMtl &);
    void PlatformFree(void);
    void Unapply(void) const;
    void CreateData(void);
    void Write(cFile &) const;
    static eOnePassModelMtl *New(cMemPool *, cBase *);
};

class eProjectedDynamicMtl {
public:
    eProjectedDynamicMtl(cBase *);
    const cType *GetType(void) const;
    void PlatformFree(void);
    void Unapply(void) const;
    void CreateData(void);
    void PlatformRead(cFile &, cMemPool *);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static eProjectedDynamicMtl *New(cMemPool *, cBase *);
};

class eProjectedHeightmapMtl {
public:
    eProjectedHeightmapMtl(cBase *);
    const cType *GetType(void) const;
    void PlatformFree(void);
    void Apply(const eCamera *, const eWorld *) const;
    void ApplyDynamic(const eCamera &, const eWorld *, const mOCS &, float, float, const mVec3 &, float, int, const eTexture *, unsigned int) const;
    void Unapply(void) const;
    void CreateData(void);
    void Write(cFile &) const;
    void PlatformRead(cFile &, cMemPool *);
    void AssignCopy(const cBase *);
    static eProjectedHeightmapMtl *New(cMemPool *, cBase *);
};

class eDrawInfo;
class eColor;
class cBase;
class cFile;
class cMemPool;

class eReflectionModelMtl {
public:
    eReflectionModelMtl(cBase *);
    const cType *GetType(void) const;
    void PlatformFree(void);
    void Unapply(void) const;
    void CreateData(void);
    void ApplyDynamic(const eDrawInfo &, const mOCS &, float, unsigned int, eColor) const;
    void PlatformRead(cFile &, cMemPool *);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
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
    ~eShadowVolumeModelMtl();
    void PlatformFree(void);
    void CreateData(void);
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    void PlatformRead(cFile &, cMemPool *);
    static eShadowVolumeModelMtl *New(cMemPool *, cBase *);
};

class eExtrudedShadowVolumeModelMtl : public eShadowVolumeModelMtl {
public:
    eExtrudedShadowVolumeModelMtl(cBase *);
    ~eExtrudedShadowVolumeModelMtl();
    static void operator delete(void *p) {
        struct DeleteRecord {
            short offset;
            short pad;
            void (*fn)(void *, void *);
        };
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        void *block = ((void **)pool)[9];
        char *allocTable = *(char **)((char *)block + 0x1C);
        DeleteRecord *rec = (DeleteRecord *)(allocTable + 0x30);
        short off = rec->offset;
        __asm__ volatile("" ::: "memory");
        void *base = (char *)block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
    void CreateData(void);
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static eExtrudedShadowVolumeModelMtl *New(cMemPool *, cBase *);
};

class eSilhouetteModelMtl {
public:
    eSilhouetteModelMtl(cBase *);
    void PlatformFree(void);
    void CreateData(void);
    void PlatformRead(cFile &, cMemPool *);
    void ApplyDynamic(const eDrawInfo &, const mOCS &, float, unsigned int, eColor) const;
    void Unapply(void) const;
    float GetSilhouetteOffset(const eDrawInfo &) const;
    void Write(cFile &) const;
    static eSilhouetteModelMtl *New(cMemPool *, cBase *);
};

#endif
