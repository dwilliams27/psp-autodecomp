#include "eVideo.h"
#include "eRenderTarget.h"
#include "eStandardSpriteMtl.h"

void eVideo::SetVideoMode(eVideoMode) {
}

int eRenderTarget::Initialize(void) {
    return 1;
}

void eRenderTarget::Uninitialize(void) {
}

void eStandardSpriteMtl::PlatformFree(void) {
}

void eStandardSpriteMtl::Unapply(void) const {
}
