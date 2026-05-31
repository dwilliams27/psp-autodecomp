// eSprite::GetTexCoord1(void) const @ 0x00038da0  (eAll_psp.obj)
//
// Returns texcoord1 (mField54/mField58). If the texture has a valid size,
// each component is scaled by the texture dimension and rounded to the
// nearest integer (away from zero); otherwise the raw coords are returned.

class cBase;
class cObject;

struct mVec2 {
    float x;
    float y;
    mVec2() {}
    mVec2(const mVec2 &);
};

class cHandle {
public:
    int _data;
};

class eSprite {
public:
    char _pad[0x44];
    int mField44;
    cHandle mField48;
    float mField4C;
    float mField50;
    float mField54;
    float mField58;
    float mField5C;
    float mField60;

    int GetTextureSize(mVec2 *) const;
    mVec2 GetTexCoord1(void) const;
};

mVec2 eSprite::GetTexCoord1(void) const {
    mVec2 result;
    mVec2 size;
    int valid = this->GetTextureSize(&size);

    float x = this->mField54;
    if (!valid) {
        result.x = x;
        result.y = this->mField58;
        return result;
    }

    float fx = x * size.x;
    float rx;
    if (fx < 0.0f) {
        rx = (float)(int)(fx - 0.5f);
    } else {
        rx = (float)(int)(fx + 0.5f);
    }
    float fy = this->mField58 * size.y;
    float ry;
    if (fy < 0.0f) {
        ry = (float)(int)(fy - 0.5f);
    } else {
        ry = (float)(int)(fy + 0.5f);
    }
    result.x = rx;
    result.y = ry;
    return result;
}
