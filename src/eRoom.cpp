class cFile;
class cBase;
class cMemPool;
class cType;
class eVolume;
class eCamera;
class eCameraBins;
class eMaterial;
template <class T> class cHandleT;

typedef int eRoom_v4sf_t __attribute__((mode(V4SF)));

struct mSphere {
    eRoom_v4sf_t v;
};

class mFrustum {
public:
    int Clip(const mSphere &, unsigned int) const;
};

inline void *operator new(unsigned int, void *p) { return p; }

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRec {
    short offset;
    short _pad;
    void (*fn)(void *, int, void *, short);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(int);
    void Write(unsigned int);
    void Write(int, const float *);
    void Write(int, const unsigned short *);
    void WriteBase(const cBase *);
    void End();
};

class cObject {
public:
    cObject(cBase *);
    void Write(cFile &) const;
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

class cBaseArray {
public:
    void Write(cWriteBlock &) const;
};

class eRoomAABBTree {
public:
    void Write(cWriteBlock &) const;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class eRoom : public cObject {
public:
    static cBase *New(cMemPool *, cBase *);
    eRoom(cBase *);
    const cType *GetType(void) const;
    void Free();
    void RemoveVolume(eVolume *);
    void ClearRoomVolumeList(eVolume *);
    void Cull(unsigned int, const eCamera &, const mFrustum &, eCameraBins *,
              int, const cHandleT<eMaterial> *, float) const;
    void Write(cFile &) const;
};

class eRoomEnvironment {
public:
    void Cull(unsigned int, const eCamera &, const mFrustum &,
              eCameraBins *) const;
};

struct GeomCallRec {
    short offset;
    short _pad;
    void (*fn)(void *, unsigned int, const eCamera &, const mFrustum &,
               eCameraBins *, unsigned int, int, const cHandleT<eMaterial> *,
               float);
};

struct UpdateCallRec {
    short offset;
    short _pad;
    void (*fn)(void *, short, void *);
};

struct eRoomCullGeom {
    char _pad0[4];
    char *typeInfo;
    char _pad8[0x38];
    mSphere sphere;
    char _pad50[0x24];
    float radiusA;
    float radiusB;
    char _pad7C[0x0C];
    unsigned int stamp;
    unsigned char flags;
    unsigned char mask;
};

struct eRoomCullNode {
    eRoomCullGeom *geom;
    char _pad4[8];
    eRoomCullNode *next;
};

class eVolumeBody {
public:
    char _pad[0x24];
    int mField24;
};

class gcUI {
public:
    void Write(cFile &) const;
};

class gcViewport {
public:
    void Write(cFile &) const;
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_000468D4;

cBase *eRoom::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eRoom *result = 0;
    eRoom *obj = (eRoom *)rec->fn(base, 0x120, 0x10, 0, 0);
    if (obj != 0) {
        new (obj) eRoom(parent);
        result = obj;
    }
    return (cBase *)result;
}

eRoom::eRoom(cBase *parent) : cObject(parent) {
    *(void **)((char *)this + 4) = (void *)0x380968;
    *(int *)((char *)this + 0x90) = 0;
    *(int *)((char *)this + 0xC0) = 0;
    *(int *)((char *)this + 0xC4) = 0;
    *(int *)((char *)this + 0xC8) = 0;
    *(int *)((char *)this + 0xCC) = 0;
    *(int *)((char *)this + 0xD0) = 0;
    *(int *)((char *)this + 0xD4) = 0;
    *(int *)((char *)this + 0xD8) = 0;
    *(eRoom **)((char *)this + 0xDC) = this;
    *(int *)((char *)this + 0xE0) = 0;
    *(int *)((char *)this + 0xE4) = 0;
    *(int *)((char *)this + 0xE8) = 0;
    *(int *)((char *)this + 0xEC) = 0;
    *(int *)((char *)this + 0xF0) = 0;
    *(unsigned char *)((char *)this + 0xF4) = 0;
    *(int *)((char *)this + 0xF8) = 0;
    *(int *)((char *)this + 0xFC) = 0;
    *(int *)((char *)this + 0x100) = 0;
    *(int *)((char *)this + 0x104) = 0;
    *(int *)((char *)this + 0x108) = 0;
    *(int *)((char *)this + 0x10C) = 0;
    __asm__ volatile(
        "lui $a0, 0x7f7f\n"
        "sw $zero, 0x110(%0)\n"
        "ori $a0, $a0, 0xffff\n"
        "sw $zero, 0x114(%0)\n"
        "mtc1 $a0, $f12\n"
        "sw $zero, 0x118(%0)\n"
        "mfc1 $a0, $f12\n"
        "mfc1 $a1, $f12\n"
        "mfc1 $a2, $f12\n"
        "mtv $a0, S120\n"
        "mtv $a1, S121\n"
        "mtv $a2, S122\n"
        "lui $a0, 0xff7f\n"
        "ori $a0, $a0, 0xffff\n"
        "mtc1 $a0, $f13\n"
        "sv.q C120, 0xA0(%0)\n"
        "mfc1 $a0, $f13\n"
        "mfc1 $a1, $f13\n"
        "mfc1 $a2, $f13\n"
        "mtv $a0, S120\n"
        "mtv $a1, S121\n"
        "mtv $a2, S122\n"
        "sv.q C120, 0xB0(%0)\n"
        :
        : "r"(this)
        : "a0", "a1", "a2", "memory");
}

const cType *eRoom::GetType(void) const {
    if (D_000468D4 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                       (const char *)0x36CD7C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                                   &cNamed::New, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                               0, 0, 0, 0);
        }
        D_000468D4 = cType::InitializeType(0, 0, 0x21C, D_000385E4,
                                           &eRoom::New,
                                           (const char *)0x36CE04,
                                           (const char *)0x36CE0C, 3);
    }
    return D_000468D4;
}

void eRoom::Free() {
    void *obj = *(void **)((char *)this + 0x118);
    if (obj != 0) {
        register DeleteRec *rec __asm__("a2") =
            (DeleteRec *)((char *)(*(void **)((char *)obj + 4)) + 0x50);
        short off = rec->offset;
        void (*fn)(void *, int, void *, short) = rec->fn;
        fn((char *)obj + off, 3, (void *)fn, off);
        *(void **)((char *)this + 0x118) = 0;
    }
    obj = *(void **)((char *)this + 0x104);
    if (obj != 0) {
        register DeleteRec *rec __asm__("a2") =
            (DeleteRec *)((char *)(*(void **)((char *)obj + 4)) + 0x50);
        short off = rec->offset;
        void (*fn)(void *, int, void *, short) = rec->fn;
        fn((char *)obj + off, 3, (void *)fn, off);
        *(void **)((char *)this + 0x104) = 0;
    }
    obj = *(void **)((char *)this + 0xC0);
    if (obj != 0) {
        register DeleteRec *rec __asm__("a2") =
            (DeleteRec *)((char *)(*(void **)((char *)obj + 4)) + 0x50);
        short off = rec->offset;
        void (*fn)(void *, int, void *, short) = rec->fn;
        fn((char *)obj + off, 3, (void *)fn, off);
        *(void **)((char *)this + 0xC0) = 0;
    }
}

void eRoom::RemoveVolume(eVolume *volume) {
    ClearRoomVolumeList(volume);
    ((eVolumeBody *)volume)->mField24 = 0;
}

void eRoom::Cull(unsigned int stamp, const eCamera &camera,
                 const mFrustum &frustum, eCameraBins *bins, int pass,
                 const cHandleT<eMaterial> *material, float alpha) const {
    eRoomCullNode *node = *(eRoomCullNode *const *)((const char *)this + 0x110);
    if (node != 0) {
        do {
            eRoomCullGeom *geom = node->geom;
            if (geom->stamp != stamp) {
                if (((geom->mask &
                      *(unsigned short *)((char *)bins + 4)) != 0) ||
                    ((*(unsigned int *)0x37D0F0 & 0x2000) != 0)) {
                    if ((geom->flags & 4) != 0) {
                        char *typeInfo = geom->typeInfo;
                        UpdateCallRec *rec = (UpdateCallRec *)(typeInfo + 0xB8);
                        short off = rec->offset;
                        void (*fn)(void *, short, void *) = rec->fn;
                        fn((char *)geom + off, off, (void *)fn);
                    }

                    float radius = geom->radiusA * geom->radiusB;
                    mSphere sphere;
                    sphere.v = geom->sphere.v;
                    *(float *)((char *)&sphere + 0xC) = radius;
                    int clip = frustum.Clip(sphere, 0x40);
                    if (clip != 0) {
                        eRoomCullGeom *callGeom = node->geom;
                        char *typeInfo = callGeom->typeInfo;
                        GeomCallRec *rec = (GeomCallRec *)(typeInfo + 0x98);
                        short off = rec->offset;
                        rec->fn((char *)callGeom + off, stamp, camera,
                                frustum, bins, clip, pass, material, alpha);
                    }
                }
            }
            node = node->next;
        } while (node != *(eRoomCullNode *const *)((const char *)this + 0x110));
    }

    eRoomCullGeom *single = *(eRoomCullGeom **)((const char *)this + 0x118);
    if (single != 0) {
        if (((single->mask &
              *(unsigned short *)((char *)bins + 4)) != 0) ||
            ((*(unsigned int *)0x37D0F0 & 0x2000) != 0)) {
            char *typeInfo = single->typeInfo;
            GeomCallRec *rec = (GeomCallRec *)(typeInfo + 0x98);
            short off = rec->offset;
            rec->fn((char *)single + off, stamp, camera, frustum, bins,
                    0x40, pass, material, alpha);
        }
    }

    if (stamp != *(unsigned int *)((const char *)this + 0xF0)) {
        *(unsigned int *)((char *)this + 0xF0) = stamp;
        unsigned int handle = *(unsigned int *)((const char *)this + 0xE8);
        bool valid;
        if (handle == 0) {
            valid = false;
        } else {
            unsigned int offset = (handle & 0xFFFF) << 2;
            char *table = (char *)0x38890;
            void *slot;
            __asm__ volatile("addu %0,%1,%2"
                             : "=r"(slot)
                             : "r"(offset), "r"(table));
            void *entry = *(void **)slot;
            void *env = 0;
            if (entry != 0) {
                if (*(unsigned int *)((char *)entry + 0x30) == handle) {
                    env = entry;
                }
            }
            valid = env != 0;
        }

        if (((unsigned int)valid & 0xFF) != 0) {
            eRoomEnvironment *env = 0;
            if (handle != 0) {
                unsigned int offset = (handle & 0xFFFF) << 2;
                char *table = (char *)0x38890;
                void *slot;
                __asm__ volatile("addu %0,%1,%2"
                                 : "=r"(slot)
                                 : "r"(offset), "r"(table));
                env = *(eRoomEnvironment **)slot;
            }
            env->Cull(stamp, camera, frustum, bins);
        } else if (*(unsigned int *)((const char *)this + 0x100) != 0) {
            unsigned int envHandle =
                *(unsigned int *)(*(char **)((const char *)this + 0xF8) + 0x58);
            eRoomEnvironment *env = 0;
            if (envHandle != 0) {
                unsigned int offset = (envHandle & 0xFFFF) << 2;
                char *table = (char *)0x38890;
                void *slot;
                __asm__ volatile("addu %0,%1,%2"
                                 : "=r"(slot)
                                 : "r"(offset), "r"(table));
                void *entry = *(void **)slot;
                if (entry != 0) {
                    if (*(unsigned int *)((char *)entry + 0x30) == envHandle) {
                        env = (eRoomEnvironment *)entry;
                    }
                }
            }
            if (env != 0) {
                env->Cull(stamp, camera, frustum, bins);
            }
        }
    }
}

void eRoom::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    ((const cObject *)this)->Write(file);

    wb.Write(*(const unsigned int *)((const char *)this + 0x90));
    wb.Write(3, (const float *)((const char *)this + 0xA0));
    wb.Write(3, (const float *)((const char *)this + 0xB0));
    wb.WriteBase(*(const cBase *const *)((const char *)this + 0xC0));

    void *arr_C4_a = *(void *const *)((const char *)this + 0xC4);
    int count_C4_a = 0;
    if (arr_C4_a != 0) {
        count_C4_a = *(int *)((char *)arr_C4_a - 4) & 0x3FFFFFFF;
    }
    wb.Write(count_C4_a);

    void *arr_C4_b = *(void *const *)((const char *)this + 0xC4);
    int count_C4_b = 0;
    if (arr_C4_b != 0) {
        count_C4_b = *(int *)((char *)arr_C4_b - 4) & 0x3FFFFFFF;
    }
    int *base_C4 = (int *)arr_C4_b;
    int i_C4 = 0;
    if (i_C4 < count_C4_b) {
        int offset_C4 = 0;
        int *p_C4 = base_C4 + offset_C4;
        do {
            ((const cHandle *)p_C4)->Write(wb);
            i_C4 += 1;
            p_C4++;
        } while (i_C4 < count_C4_b);
    }

    ((const cHandle *)((const char *)this + 0xCC))->Write(wb);
    ((const eRoomAABBTree *)((const char *)this + 0xD0))->Write(wb);

    void *arr_E0_a = *(void *const *)((const char *)this + 0xE0);
    int count_E0_a = 0;
    if (arr_E0_a != 0) {
        count_E0_a = *(int *)((char *)arr_E0_a - 4) & 0x3FFFFFFF;
    }
    wb.Write(count_E0_a);

    void *arr_E0_b = *(void *const *)((const char *)this + 0xE0);
    int count_E0_b = 0;
    if (arr_E0_b != 0) {
        count_E0_b = *(int *)((char *)arr_E0_b - 4) & 0x3FFFFFFF;
    }
    int *base_E0 = (int *)arr_E0_b;
    int i_E0 = 0;
    if (i_E0 < count_E0_b) {
        int offset_E0 = 0;
        int *p_E0 = base_E0 + offset_E0;
        do {
            ((const cHandle *)p_E0)->Write(wb);
            i_E0 += 1;
            p_E0++;
        } while (i_E0 < count_E0_b);
    }

    void *arr_C8_a = *(void *const *)((const char *)this + 0xC8);
    int count_C8_a = 0;
    if (arr_C8_a != 0) {
        count_C8_a = *(int *)((char *)arr_C8_a - 4) & 0x3FFFFFFF;
    }
    wb.Write(count_C8_a);

    void *arr_C8_b = *(void *const *)((const char *)this + 0xC8);
    int count_C8_b = 0;
    if (arr_C8_b != 0) {
        count_C8_b = *(int *)((char *)arr_C8_b - 4) & 0x3FFFFFFF;
    }
    wb.Write(count_C8_b, (const unsigned short *)arr_C8_b);

    wb.Write(3, (const float *)((const char *)this + 0x80));
    wb.Write(3, (const float *)((const char *)this + 0x50));
    wb.Write(3, (const float *)((const char *)this + 0x60));
    wb.Write(3, (const float *)((const char *)this + 0x70));

    void *arr_E4_a = *(void *const *)((const char *)this + 0xE4);
    int count_E4_a = 0;
    if (arr_E4_a != 0) {
        count_E4_a = *(int *)((char *)arr_E4_a - 4) & 0x3FFFFFFF;
    }
    wb.Write(count_E4_a);

    void *arr_E4_b = *(void *const *)((const char *)this + 0xE4);
    int count_E4_b = 0;
    if (arr_E4_b != 0) {
        count_E4_b = *(int *)((char *)arr_E4_b - 4) & 0x3FFFFFFF;
    }
    int *base_E4 = (int *)arr_E4_b;
    int i_E4 = 0;
    if (i_E4 < count_E4_b) {
        int offset_E4 = 0;
        int *p_E4 = base_E4 + offset_E4;
        do {
            ((const cHandle *)p_E4)->Write(wb);
            i_E4 += 1;
            p_E4++;
        } while (i_E4 < count_E4_b);
    }

    ((const cHandle *)((const char *)this + 0xEC))->Write(wb);
    ((const cBaseArray *)((const char *)this + 0xD8))->Write(wb);
    wb.WriteBase(*(const cBase *const *)((const char *)this + 0x104));

    void *p_118_a = *(void *const *)((const char *)this + 0x118);
    wb.Write((bool)(p_118_a != 0));

    void *p_118_b = *(void *const *)((const char *)this + 0x118);
    if (p_118_b != 0) {
        wb.WriteBase((const cBase *)p_118_b);
    }

    wb.End();
}

void gcUI::Write(cFile &f) const {
    cWriteBlock wb(f, 1);
    wb.End();
}

void gcViewport::Write(cFile &f) const {
    cWriteBlock wb(f, 1);
    wb.End();
}

extern "C" {

extern void *_impure_ptr;
void *_fopen_r(void *, const char *, const char *);
double _strtod_r(void *, const char *, char **);

void *fopen(const char *file, const char *mode) {
    return _fopen_r(_impure_ptr, file, mode);
}

double strtod(const char *s, char **endptr) {
    return _strtod_r(_impure_ptr, s, endptr);
}

}
