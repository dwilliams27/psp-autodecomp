// ODR-WARNING: split translation unit local redeclaration for gcUIDialog::Draw only.

typedef unsigned int SceULong128 __attribute__((mode(TI)));

struct mOCS {
    SceULong128 row0;
    SceULong128 row1;
    SceULong128 row2;
    SceULong128 row3;
};

struct eRect {
    float x;
    float y;
    float w;
    float h;
};

struct eViewport;

class eCamera {
public:
    char _data[0x310];

    eCamera(void);
    void MakeOrtho(const eCamera *);
    void GetOrthoRect(eRect *) const;
};

class eVideo {
public:
    static void SetViewport(const eViewport *);
    static void SetCamera(const eCamera &);
};

class gcUIDialog {
public:
    void Draw(unsigned int);
    void Draw(const eCamera &, const mOCS &, const eRect &, unsigned int);
};

extern mOCS D_00038578;
extern int D_000385B8;

void gcUIDialog::Draw(unsigned int flags) {
    mOCS callOCS;
    eCamera camera;
    camera.MakeOrtho(0);
    eVideo::SetViewport(0);
    eVideo::SetCamera(camera);

    eRect rect;
    int identityScratch[28];
    camera.GetOrthoRect(&rect);

    if (D_000385B8 == 0) {
        void *identity = &D_00038578;
        int initialized = 1;
        __asm__ volatile("mtc1 $0, $f12" ::: "$f12");
        D_000385B8 = initialized;
        __asm__ volatile(
            "mfc1 $t0, $f12\n"
            "mfc1 $a2, $f12\n"
            "mfc1 $a3, $f12\n"
            "mtv $a2, S120\n"
            "mtv $a3, S121\n"
            "mtv $t0, S122\n"
            "lui $a2, 0x3f80\n"
            "sv.q C120, 0x360($sp)\n"
            "mfc1 $a3, $f12\n"
            "mtc1 $a2, $f13\n"
            "mfc1 $a2, $f13\n"
            "mfc1 $t0, $f12\n"
            "mtv $a2, S130\n"
            "mtv $a3, S131\n"
            "mtv $t0, S132\n"
            "sv.q C130, 0x3a0($sp)\n"
            "mfc1 $a2, $f12\n"
            "mfc1 $a3, $f13\n"
            "mfc1 $t0, $f12\n"
            "mtv $a2, S200\n"
            "mtv $a3, S201\n"
            "mtv $t0, S202\n"
            "sv.q C200, 0x3b0($sp)\n"
            "mfc1 $a2, $f12\n"
            "mfc1 $a3, $f12\n"
            "mfc1 $t0, $f13\n"
            "mtv $a2, S210\n"
            "mtv $a3, S211\n"
            "mtv $t0, S212\n"
            "sv.q C210, 0x3c0($sp)\n"
            "sv.q C130, 0x370($sp)\n"
            "sv.q C200, 0x380($sp)\n"
            "sv.q C210, 0x390($sp)\n"
            "sv.q C130, -0x7a88($a1)\n"
            "sv.q C200, 0x10(%0)\n"
            "sv.q C210, 0x20(%0)\n"
            "sv.q C120, 0x30(%0)\n"
            :
            : "r"(identity), "m"(identityScratch)
            : "$a2", "$a3", "$t0", "$f12", "$f13", "memory");
    }

    callOCS = D_00038578;
    Draw(camera, callOCS, rect, flags);
}
