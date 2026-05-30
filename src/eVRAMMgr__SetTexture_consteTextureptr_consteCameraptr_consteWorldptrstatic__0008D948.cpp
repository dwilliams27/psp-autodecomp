// eVRAMMgr::SetTexture(const eTexture *, const eCamera *, const eWorld *) static
// Address: 0x0008d948, Size: 188B  Obj: eAll_psp.obj

class eCamera;
class eWorld;

struct eTextureSlot {
    short offset;       // +0x00
    short pad;          // +0x02
    int (*fn)(void *, int, const eCamera *, const eWorld *); // +0x04
};

struct eTextureDesc {
    char _pad[0xA0];
    eTextureSlot slot;  // 0xA0
};

class eTexture {
public:
    int _pad0;          // 0x00
    eTextureDesc *desc; // 0x04
};

class eVRAMMgr {
public:
    static int SetTexture(const eTexture *, const eCamera *, const eWorld *);
};

struct GpuCmdList {
    int pad[2];
    int *ptr;
};

extern GpuCmdList D_000984D0;

#pragma control sched=1
int eVRAMMgr::SetTexture(const eTexture *tex, const eCamera *camera, const eWorld *world) {
    const eTexture **base = (const eTexture **)0x380000;
    if (tex == base[-2865]) {
        return 0;
    }
    if (tex != 0) {
        eTextureSlot *s = &((eTexture *)tex)->desc->slot;
        void *arg0 = (char *)tex + s->offset;
        __asm__ volatile("" ::: "memory");
        s->fn(arg0, 0, camera, world);
    } else {
        int *flag = (int *)0x98428;
        if (flag[19] != 0) {
            int v = 0x1E000000;
            __asm__ volatile("" : "+r"(v));
            GpuCmdList *cl = (GpuCmdList *)0x984D0;
            int *p = cl->ptr;
            cl->ptr = p + 1;
            *p = v;
            v = 0;
            flag[19] = v;
        }
    }
    base[-2865] = tex;
    return 1;
}
