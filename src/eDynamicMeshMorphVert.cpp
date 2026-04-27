class cFileSystem {
public:
    static void Read(void *handle, void *buf, unsigned int size);
};

class cReadBlock {
public:
    int _data[5];
};

struct eDynamicMeshMorphVert {
    char _data[8];
    void Read(cReadBlock &);
};

#pragma control sched=1
// eDynamicMeshMorphVert::Read(cReadBlock &) — 0x0008ef7c
void eDynamicMeshMorphVert::Read(cReadBlock &rb) {
    __asm__ volatile("" ::: "memory");
    cFileSystem::Read(*(void **)rb._data[0], this, 6);
    __asm__ volatile("" ::: "memory");
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 6, 2);
}
#pragma control sched=2
