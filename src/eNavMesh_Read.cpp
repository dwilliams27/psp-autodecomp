// ODR-WARNING: this TU intentionally redeclares eNavMesh with a minimal
// member list because include/eNavMesh.h does not yet declare Read. Keep this
// split from src/eNavMesh.cpp so existing matches in the canonical TU are not
// perturbed.

class cBase;
class cFile;
class cMemPool;

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);

    template <class T> void Read(T &);
};

class cFileSystem {
public:
    static void Read(void *, void *, unsigned int);
};

class cObject {
public:
    int Read(cFile &, cMemPool *);
};

class eNavTri;
class eNavVert;
class eNavPortal;

template <class T>
class cArrayBase {
public:
    void *mData;
    bool SetSize(int, const cArrayBase &);
};

template <class T>
class cArray : public cArrayBase<T> {
public:
    void Read(cReadBlock &);
};

class eNavMesh : public cObject {
public:
    int Read(cFile &, cMemPool *);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);

#pragma control sched=1

// -- eNavMesh::Read(cFile &, cMemPool *) @ 0x0005750c --
int eNavMesh::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$17");
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 3, true);

    if ((unsigned int)rb._data[3] != 3) goto fail;
    if (((cObject *)this)->Read(file, pool)) goto success;

fail:
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;

success:
    char empty[8];
    int count;
    {
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read(h, &count, 4);
    }

    register cArrayBase<eNavTri> *tris __asm__("$4") =
        (cArrayBase<eNavTri> *)((char *)this + 0x44);
    register int size __asm__("$5") = count;
    __asm__ volatile("" : "+r"(tris), "+r"(size));
    empty[6] = 0;
    register int fill __asm__("$6");
    __asm__ volatile("ori %0, $0, 0xff" : "=r"(fill));
    empty[7] = fill;
    empty[2] = fill;
    empty[1] = fill;
    empty[0] = fill;
    empty[5] = fill;
    empty[4] = fill;
    empty[3] = fill;
    tris->SetSize(size, *(const cArrayBase<eNavTri> *)empty);

    int total = count;
    int index = 0;
    char *tri = *(char **)((char *)this + 0x44);
    if (index < total) {
        register int offset __asm__("$4");
        __asm__ volatile("ori %0, $0, 0" : "=r"(offset));
        tri += offset;
        do {
            {
                void *h = *(void **)rb._data[0];
                __asm__ volatile("" : "+r"(h));
                cFileSystem::Read(h, tri, 3);
            }
            {
                void *h = *(void **)rb._data[0];
                __asm__ volatile("" : "+r"(h));
                cFileSystem::Read(h, tri + 3, 3);
            }
            {
                void *h = *(void **)rb._data[0];
                __asm__ volatile("" : "+r"(h));
                cFileSystem::Read(h, tri + 6, 1);
            }
            {
                void *h = *(void **)rb._data[0];
                __asm__ volatile("" : "+r"(h));
                cFileSystem::Read(h, tri + 7, 1);
            }
            index += 1;
            tri += 8;
        } while (index < total);
    }

    ((cArray<eNavVert> *)((char *)this + 0x48))->Read(rb);
    rb.Read(*(cArray<unsigned char> *)((char *)this + 0x4C));
    {
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read(h, (char *)this + 0x50, 0xC);
    }
    {
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read(h, (char *)this + 0x60, 0xC);
    }
    {
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read(h, (char *)this + 0x70, 0xC);
    }
    ((cArray<eNavPortal> *)((char *)this + 0x80))->Read(rb);
    return result;
}

#pragma control sched=2
