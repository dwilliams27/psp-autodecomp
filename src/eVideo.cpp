#include "eVideo.h"
#include "eVRAMMgr.h"

class eVideoPlatform {
public:
    static void Flip(bool, bool);
};

class eIVideoRegisterInit {
public:
    eIVideoRegisterInit *prev;
    eIVideoRegisterInit *next;
};

extern eIVideoRegisterInit *D_0037D130;

struct GpuCmdList {
    int pad[2];
    int *ptr;
};

struct eVideoConfig {
    char pad[0x28];
    unsigned char useVirtual;
    unsigned char keepAspect;
    char pad2[2];
    unsigned int width;
    int height;
};

extern GpuCmdList D_000984D0;

void eVideo::EndStencil(void) {
}

void eVideo::ApplyVertexShader(const eCamera *, const eVertexShader *) {
}

void eVideo::SetAlphaWriteEnable(bool) {
}

void eVideo::SetFlippedBFCMode(bool) {
}

int eVideo::IsWidescreen(void) {
    return 0;
}

void eVideo::SetVideoMode(eVideoMode) {
}

void eVideo::InvalidateTextureCache(void) {
    eVRAMMgr::InvalidateAll();
}

void eVideo::SetDefaultVideoMode(void) {
    eVideo::SetVideoMode((eVideoMode)0);
}

void eVideo::GetVirtualWidthHeight(int *width, int *height) {
    eVideo::GetMaxWidthHeight(width, height);

    eVideoConfig *config = *(eVideoConfig **)0x37D0C8;
    if (config != 0 && config->useVirtual != 0) {
        if (config->keepAspect != 0) {
            eVideo::GetAspectWidthHeight(config->width, config->height, width, height);
        } else {
            *width = config->width;
            *height = config->height;
        }
    }
}

void eVideo::GetAspectWidthHeight(int width, int height, int *outWidth, int *outHeight) {
    int currentWidth = *outWidth;
    int currentHeight = *outHeight;

    if (((float)currentWidth / (float)currentHeight) < ((float)width / (float)height)) {
        unsigned int scaledHeight = width;
        scaledHeight *= currentHeight;
        *outHeight = (int)scaledHeight / currentWidth;
        *outWidth = width;
        return;
    }

    unsigned int scaledWidth = height;
    scaledWidth *= currentWidth;
    *outWidth = (int)scaledWidth / currentHeight;
    *outHeight = height;
    __asm__ volatile("" ::: "memory");
}

void eVideo::Flip(void) {
    eVideoPlatform::Flip(false, false);
}

void eVideo::EndFrame(void) {
    eVRAMMgr::EndRender();
    int *p = D_000984D0.ptr;
    D_000984D0.ptr = p + 2;
    p[0] = 0x0F000000;
    p[1] = 0x0C000000;
}

void eVideo::RegisterInit(eIVideoRegisterInit *p) {
    eIVideoRegisterInit *head = D_0037D130;
    if (head != 0) {
        p->next = head->next;
        p->prev = D_0037D130;
        p->next->prev = p;
        p->prev->next = p;
    } else {
        D_0037D130 = p;
        p->prev = p;
        p->next = p;
    }
}
