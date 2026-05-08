class cBase;
class cFile;
class ePoint;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

template <class T>
class cHandleT {
public:
    int mIndex;
};

template <class T> T *dcast(const cBase *);

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

void cFile_SetCurrentPos(void *, unsigned int);

extern "C" void *__vec_new(void *arr, int count, int size, void (*ctor)(void *));

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcSubGeomController {
public:
    char _pad[0x24];

    gcSubGeomController(cBase *parent);
    ~gcSubGeomController();
    void Reset(cMemPool *pool, bool flag);
    void Write(cFile &file) const;
    int Read(cFile &file, cMemPool *pool);
};

class gcGeomCurveController : public gcSubGeomController {
public:
    cHandleT<ePoint> m_target;   // 0x24
    int m_pad28;                 // 0x28
    short m_flag;                // 0x2C
    short m_pad2E;               // 0x2E
    char m_arr30[8];             // 0x30..0x37

    gcGeomCurveController(cBase *parent);
    ~gcGeomCurveController();
    void Reset(cMemPool *pool, bool flag);
    void Write(cFile &file) const;
    int Read(cFile &file, cMemPool *pool);
    void SetTarget(cHandleT<ePoint> p);
    void AssignCopy(const cBase *base);
    static gcGeomCurveController *New(cMemPool *pool, cBase *parent);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

// Free-function wrapper declaration so New can call the ctor via an unresolved
// symbol (relocation will be masked during byte comparison).
void gcGeomCurveController_ctor(gcGeomCurveController *self, cBase *parent);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct gcc_half3 {
    short a;
    short b;
    short c;
};

gcGeomCurveController::~gcGeomCurveController() {
    *(void **)((char *)this + 4) = (void *)0x38BF20;
    *(int *)((char *)this + 8) = 0;
}

void gcGeomCurveController::SetTarget(cHandleT<ePoint> p) {
    m_flag = 1;
    m_target = p;
    cHandleT<ePoint> *p2 = (cHandleT<ePoint> *)((char *)this + 0x28);
    p2->mIndex = 0;
}

void gcGeomCurveController::Reset(cMemPool *pool, bool flag) {
    if (flag) {
        gcSubGeomController::Reset(pool, flag);
    }
}

void gcGeomCurveController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcSubGeomController::Write(file);
    wb.End();
}

int gcGeomCurveController::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19");
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && gcSubGeomController::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

gcGeomCurveController::gcGeomCurveController(cBase *parent) : gcSubGeomController(parent) {
    *(void **)((char *)this + 4) = (void *)0x38BF20;
    m_target.mIndex = 0;
    m_pad28 = 0;
    m_flag = 0;
    m_pad2E = -1;
    __vec_new((char *)this + 0x30, 1, 6, (void (*)(void *))0x24400C);
}

gcGeomCurveController *gcGeomCurveController::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcGeomCurveController *result = 0;
    gcGeomCurveController *obj = (gcGeomCurveController *)entry->fn(base, 0x38, 4, 0, 0);
    if (obj != 0) {
        gcGeomCurveController_ctor(obj, parent);
        result = obj;
    }
    return result;
}

void gcGeomCurveController::AssignCopy(const cBase *base) {
    gcGeomCurveController *other = dcast<gcGeomCurveController>(base);
    *(int *)((char *)this + 8) = *(int *)((char *)other + 8);
    *(int *)((char *)this + 0xC) = *(int *)((char *)other + 0xC);
    *(float *)((char *)this + 0x10) = *(float *)((char *)other + 0x10);
    *(float *)((char *)this + 0x14) = *(float *)((char *)other + 0x14);

    int i = 0;
    gcc_half3 *dst = (gcc_half3 *)((char *)this + 0x1C);
    gcc_half3 *src = (gcc_half3 *)((char *)other + 0x1C);
    *(float *)((char *)this + 0x18) = *(float *)((char *)other + 0x18);
    do {
        short a = src->a;
        short b = src->b;
        short c = src->c;
        __asm__ volatile("" ::: "memory");
        dst->a = a;
        dst->b = b;
        dst->c = c;
        i += 1;
        dst += 1;
        src += 1;
    } while (i <= 0);

    int *src24 = (int *)((char *)other + 0x24);
    int value24 = *src24;
    int *dst24 = (int *)((char *)this + 0x24);
    int *src28 = (int *)((char *)other + 0x28);
    int *dst28 = (int *)((char *)this + 0x28);
    *dst24 = value24;
    int value28 = *src28;
    *dst28 = value28;
    *(unsigned short *)((char *)this + 0x2C) =
        *(unsigned short *)((char *)other + 0x2C);
    *(short *)((char *)this + 0x2E) = *(short *)((char *)other + 0x2E);

    int j = 0;
    gcc_half3 *dst2 = (gcc_half3 *)((char *)this + 0x30);
    gcc_half3 *src2 = (gcc_half3 *)((char *)other + 0x30);
    do {
        short a = src2->a;
        short b = src2->b;
        short c = src2->c;
        __asm__ volatile("" ::: "memory");
        dst2->a = a;
        dst2->b = b;
        dst2->c = c;
        j += 1;
        dst2 += 1;
        src2 += 1;
    } while (j <= 0);
}
