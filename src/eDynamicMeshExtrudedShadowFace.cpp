// -----------------------------------------------------------------------------
// eDynamicMeshExtrudedShadowFace::Read(cReadBlock &)  @ 0x0004d324
//   (eAll_psp.obj, 44B)
// -----------------------------------------------------------------------------

class cFileSystem {
public:
    static void Read(void *, void *, unsigned int);
};

class cReadBlock {
public:
    int _data[1];
};

class eDynamicMeshExtrudedShadowFace {
public:
    void Read(cReadBlock &);
};

void eDynamicMeshExtrudedShadowFace::Read(cReadBlock &rb) {
    __asm__ volatile("" ::: "memory");
    void *handle = *(void **)rb._data[0];
    cFileSystem::Read(handle, this, 6);
}
