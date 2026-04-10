#include "eRenderTarget.h"

// ── Flip ──

extern int g_flipCount;

void eRenderTarget::Flip(void) {
    g_flipCount += 1;
}

// ── Constructor ──

extern char eRenderTargetclassdesc[];

eRenderTarget::eRenderTarget(cBase *parent) : eTexture(parent) {
    ((void **)this)[1] = eRenderTargetclassdesc;
    field_4C = 0;
    field_4D = 0;
    field_4E = 0;
    field_4F = 0;
    field_50 = 0;
    field_54 = 0;
    field_58 = 0;
}

// ── Write ──

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned char);
    void End(void);
};

void eRenderTarget::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    eTexture::Write(file);
    wb.Write(field_4C);
    wb.Write(field_4D);
    wb.Write(field_4E);
    wb.End();
}

// ── Apply ──

class eRenderSurface;

class eVRAMMgrHelper {
public:
    static void SetTextureFromSurface(const eRenderSurface *, bool);
};

struct RenderState {
    char pad[0x4C];
    int textureFlag;
};

extern RenderState D_00098428;

struct GpuCmdList {
    int pad[2];
    int *ptr;
};

extern GpuCmdList D_000984D0;

void eRenderTarget::Apply(int, const eCamera *, const eWorld *) const {
    unsigned char flags = ((const unsigned char *)this)[0x47];
    if (flags & 2) {
        eVRAMMgrHelper::SetTextureFromSurface(*(const eRenderSurface **)field_50, true);
    } else {
        int flag = D_00098428.textureFlag;
        if (flag != 0) {
            int *p = D_000984D0.ptr;
            D_000984D0.ptr = p + 1;
            *p = 0x1E000000;
            flag = 0;
            D_00098428.textureFlag = flag;

        }
    }
}

// ── New ──

void eRenderTarget_eRenderTarget(eRenderTarget *, cBase *);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

eRenderTarget *eRenderTarget::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    eRenderTarget *result = 0;
    eRenderTarget *obj = (eRenderTarget *)entry->fn(base, 0x5C, 4, 0, 0);
    if (obj != 0) {
        eRenderTarget_eRenderTarget(obj, parent);
        result = obj;
    }
    return result;
}

// ── Static stubs ──

int eRenderTarget::Initialize(void) {
    return 1;
}

void eRenderTarget::Uninitialize(void) {
}
