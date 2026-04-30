// eSurfaceProperty.cpp — decompiled from eAll_psp.obj
// eSurfaceProperty inherits from cNamed.
//   0x000536a4  eSurfaceProperty::Read(cFile &, cMemPool *)             (208B)
//   0x001f5f84  eSurfaceProperty::AssignCopy(const cBase *)             (112B)

class cBase;
class cFile;
class cMemPool;

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cFileSystem {
public:
    static void Read(void *handle, void *buf, unsigned int size);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned int);
    void End(void);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);

template <class T> T *dcast(const cBase *);

class cNamed {
public:
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
};

class eSurfaceProperty : public cNamed {
public:
    int Read(cFile &, cMemPool *);
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
};

#pragma control sched=1

// ── eSurfaceProperty::Write(cFile &) const @ 0x0005364c ──
void eSurfaceProperty::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cNamed::Write(file);
    wb.Write(*(const unsigned int *)((const char *)this + 0x20));
    wb.End();
}

// ── eSurfaceProperty::Read(cFile &, cMemPool *) @ 0x000536a4 ──
int eSurfaceProperty::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 && ((cNamed *)this)->Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    {
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" ::: "memory");
        cFileSystem::Read(h, (char *)this + 0x20, 4);
    }
    return result;
}

// ── eSurfaceProperty::AssignCopy(const cBase *) @ 0x001f5f84 ──
#pragma control sched=1
void eSurfaceProperty::AssignCopy(const cBase *base) {
    struct cName { int m[6]; };
    struct Tail { int x; };
    eSurfaceProperty *other = dcast<eSurfaceProperty>(base);
    *(cName *)((char *)this + 8) = *(const cName *)((char *)other + 8);
    __asm__ volatile("" ::: "memory");
    *(Tail *)((char *)this + 0x20) = *(const Tail *)((char *)other + 0x20);
}
