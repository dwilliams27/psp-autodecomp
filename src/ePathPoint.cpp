class cFile;

class cReadBlock {
public:
    int _data[5];
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(float);
    void Write(int, const float *);
    void End(void);
};

class cFileSystem {
public:
    static void Read(void *handle, void *buf, unsigned int size);
};

struct ePathPoint {
    float pos[3];
    float pad;
    float yaw;
    float pitch;
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
};

#pragma control sched=1
// ePathPoint::Write(cWriteBlock &) const — 0x0005aba8
void ePathPoint::Write(cWriteBlock &wb) const {
    wb.Write(3, pos);
    wb.Write(yaw);
    wb.Write(pitch);
}

// ePathPoint::Read(cReadBlock &) — 0x0005abfc
void ePathPoint::Read(cReadBlock &rb) {
    __asm__ volatile("" ::: "memory");
    cFileSystem::Read(*(void **)rb._data[0], (char *)this, 12);
    __asm__ volatile("" ::: "memory");
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 16, 4);
    __asm__ volatile("" ::: "memory");
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 20, 4);
    if (yaw < -180.0f || !(yaw <= 180.0f)) {
        yaw = 0.0f;
    }
}
#pragma control sched=2
