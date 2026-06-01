// eSprite::GetTextureSize(mVec2 *) const @ 0x0003857c  (eAll_psp.obj)
//
// Fetches the bound texture via GetTexture(). If null, writes (0,0) to the
// out vector and returns 0. Otherwise reads the texture width/height (signed
// shorts at +0x48 / +0x4A), converts to float, stores them and returns 1.

struct mVec2 {
    float x;
    float y;
};

struct eTexture {
    char _pad[0x48];
    short mWidth;   // +0x48
    short mHeight;  // +0x4A
};

class eSprite {
public:
    const eTexture *GetTexture(void) const;
    int GetTextureSize(mVec2 *) const;
};

int eSprite::GetTextureSize(mVec2 *out) const {
    const eTexture *tex = this->GetTexture();
    if (tex != 0) {
        float w = (float)tex->mWidth;
        float h = (float)tex->mHeight;
        out->x = w;
        out->y = h;
        return 1;
    }
    out->x = 0.0f;
    out->y = 0.0f;
    return 0;
}
