// gcMap::Draw(void) @ 0x000f31a4
//
// Split TU per AGENTS.md SPLIT-TU rule. The main gcMap definition lives in
// include/gcMap.h / src/gcMap.cpp, but that header does not declare Draw.
//
// ODR-WARNING: local redeclarations expose only the method and field offsets
// used here.

class eCamera;
class eCameraBins;
class eMaterial;
class eWorld;
struct eViewport;
struct mFrustum;
struct mVec3;

template <class T>
class cHandleT {
public:
    int mIndex;
};

struct eViewport {
    int x;
    int y;
    int w;
    int h;
    int pad0;
    int pad1;
};

class eCamera {
public:
    void MakeOrtho(const eCamera *);
};

class eVideo {
public:
    static void GetMaxWidthHeight(int *, int *);
    static void BeginFrame(void);
    static void EndFrame(void);
    static void SetFogInfo(const eCamera *);
    static void Clear(bool, bool, unsigned int, float, const eViewport *);
    static void SetViewport(const eViewport *);
    static void SetFullscreenViewport(const eViewport &);
};

class eCameraBins {
public:
    char pad_00000[0x17714];
    int mCameraCount;                 // 0x17714
    eCamera *mCameraPtrs[6];          // 0x17718
    char mCameras[4][0x310];          // 0x17730
    char pad_18370[0x90];             // 0x18370
    unsigned char mCurrentViewport;   // 0x18400

    eCameraBins(unsigned short, const eWorld *);
    void Draw(bool);
};

class eWorld {
public:
    void Cull(const eCamera &, const mFrustum &, const mVec3 &, eCameraBins *,
              int, const cHandleT<eMaterial> *, float) const;
};

class gcViewport {
public:
    static void Clear(void);
    static void Draw(void);
};

class gcMap {
public:
    void Draw(void);
};

extern char *D_0037D840;

void gcMap::Draw(void) {
    int drawBlocked = (*(int *)((char *)this + 0x140) & 0x10000) != 0;
    drawBlocked &= 0xFF;
    if (drawBlocked != 0) {
        *(int *)((char *)this + 0x140) &= 0xFFFEFFFF;
        return;
    }

    int width;
    int height;
    eViewport viewport;
    eCameraBins bins(0x81, *(eWorld **)((char *)this + 0x50));

    eVideo::GetMaxWidthHeight(&width, &height);

    viewport.x = 0;
    viewport.y = 0;
    viewport.w = width;
    viewport.h = height;
    eVideo::SetFullscreenViewport(viewport);

    int i = 0;
    char **viewportRoot = (char **)0x37D840;
    int offset = 0;
    do {
        char *base = 0;
        char *viewports = *viewportRoot;
        if (viewports != 0) {
            base = viewports + offset;
        }

        if (((unsigned char *)base)[0x1380] != 0 &&
            ((unsigned char *)base)[0x1381] != 0) {
            int count = bins.mCameraCount;
            bins.mCurrentViewport = i;
            eCamera *camera = (eCamera *)(base + 0xEE0);
            bins.mCameraPtrs[count] = camera;

            eCamera *ortho = (eCamera *)bins.mCameras[count];
            ortho->MakeOrtho(camera);

            eWorld *world = *(eWorld **)((char *)this + 0x50);
            bins.mCameraCount = bins.mCameraCount + 1;
            world->Cull(*camera, *(mFrustum *)((char *)camera + 0x80),
                        *(mVec3 *)((char *)camera + 0x110), &bins, 0, 0, 1.0f);
        }

        i++;
        offset += 0x1390;
    } while (i < 5);

    eVideo::BeginFrame();
    eVideo::SetViewport(0);
    eVideo::Clear(true, true, 0, 1.0f, 0);
    gcViewport::Clear();
    bins.Draw(true);
    eVideo::SetFogInfo(0);
    gcViewport::Draw();
    eVideo::EndFrame();
}
