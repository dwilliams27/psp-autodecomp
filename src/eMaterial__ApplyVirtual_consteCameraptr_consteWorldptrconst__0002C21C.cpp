// eMaterial::ApplyVirtual(const eCamera *, const eWorld *) const  — 0x0002c21c
// Split-TU: eMaterial is not declared with ApplyVirtual in include/*.h; declare locally.

class eCamera;
class eWorld;
class eTexture;

// eVRAMMgr::SetTexture(const eTexture *, const eCamera *, const eWorld *) static
//   -> __0fIeVRAMMgrKSetTexturePC6IeTexturePC6HeCameraPC6GeWorldT
class eVRAMMgr {
public:
    static void SetTexture(const eTexture *, const eCamera *, const eWorld *);
};

extern void *D_00038890[];      // global handle-pointer table

extern int   D_0037D0E8;        // last-applied camera
extern void *D_0037D33C;        // current bound texture
extern int   D_0037D134;        // texture-state scratch

class eMaterial {
public:
    char _pad[0x48];
    int mTextures[2];           // 0x48 (cHandleT<eTexture>[2])
    void ApplyVirtual(const eCamera *, const eWorld *) const;
};

void eMaterial::ApplyVirtual(const eCamera *camera, const eWorld *world) const {
    int value = mTextures[0];
    eTexture *resolved;
    if (value == 0) {
        resolved = 0;
    } else {
        void *entry = D_00038890[value & 0xFFFF];
        resolved = 0;
        if (entry != 0) {
            if (*(int *)((char *)entry + 0x30) == value) {
                resolved = (eTexture *)entry;
            }
        }
    }

    __asm__ volatile("" ::: "memory");
    eTexture *texture = resolved;
    if (texture != 0 && (*((unsigned char *)texture + 0x47) & 1)) {
        if (D_0037D0E8 != (int)camera) {
            if (D_0037D33C == texture) {
                eVRAMMgr::SetTexture(0, 0, 0);
            }
            D_0037D0E8 = (int)camera;
        }
        D_0037D134 = 0;
        eVRAMMgr::SetTexture(texture, camera, world);
        D_0037D134 = 0;
    }
}
