// eTextureFilter: texture filter base class with two cHandle members.
// Matches:
//   eTextureFilter::eTextureFilter(cBase *)            — 0x0007FB70
//   eTextureFilter::Write(cFile &) const               — 0x0007FA50
//   eTextureFilter::Read(cFile &, cMemPool *)          — 0x0007FA9C
//   eTextureFilter::~eTextureFilter(void)              — 0x0007FB90
//   eTextureFilter::GetType(void) const                — 0x00216EAC

class cBase;
class cFile;
class cMemPool;
class cType;

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cHandle {
public:
    int mIndex;
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &, cMemPool *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void *cMemPool_GetPoolFromPtr(const void *);
extern "C" void free(void *);

extern char eTextureFilterclassdesc[];
extern char eTextureFiltervirtualtable[];
extern const char eTextureFilter_base_name[];
extern const char eTextureFilter_base_desc[];

static cType *type_base;
static cType *type_eTextureFilter;

class eTextureFilter {
public:
    cBase *m_parent;    // offset 0
    void *m_classdesc;  // offset 4
    cHandle m_handle1;  // offset 8  (4 bytes)
    cHandle m_handle2;  // offset 12 (4 bytes)

    eTextureFilter(cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    const cType *GetType(void) const;
};

// ── Constructor ──

eTextureFilter::eTextureFilter(cBase *parent) {
    m_parent = parent;
    ((void **)this)[1] = eTextureFilterclassdesc;
    m_handle1.mIndex = 0;
    m_handle2.mIndex = 0;
}

// ── Write ──

void eTextureFilter::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    m_handle1.Write(wb);
    m_handle2.Write(wb);
    wb.End();
}

// ── Read ──

int eTextureFilter::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    cReadBlock rb(file, 1, true);
    if (rb._data[3] != 1) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return 0;
    }
    m_handle1.mIndex = 0;
    m_handle1.Read(rb, (cMemPool *)cMemPool_GetPoolFromPtr(&m_handle1));
    m_handle2.mIndex = 0;
    m_handle2.Read(rb, (cMemPool *)cMemPool_GetPoolFromPtr(&m_handle2));
    return result;
}

// ── GetType ──

const cType *eTextureFilter::GetType(void) const {
    if (!type_eTextureFilter) {
        if (!type_base) {
            type_base = cType::InitializeType(eTextureFilter_base_name,
                                              eTextureFilter_base_desc,
                                              1, 0, 0, 0, 0, 0);
        }
        type_eTextureFilter = cType::InitializeType(0, 0, 0x13B, type_base,
                                                    0, 0, 0, 0);
    }
    return type_eTextureFilter;
}

// ── Destructor ──

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

extern "C" {

void eTextureFilter___dtor_eTextureFilter_void(eTextureFilter *self, int flags) {
    if (self != 0) {
        *(void **)((char *)self + 4) = eTextureFiltervirtualtable;
        if (flags & 1) {
            void *pool = cMemPool_GetPoolFromPtr(self);
            if (pool != 0) {
                void *block = *(void **)((char *)pool + 0x24);
                char *allocTable = *(char **)((char *)block + 0x1C);
                DeleteRecord *rec = (DeleteRecord *)(allocTable + 0x30);
                short off = rec->offset;
                void *base = (char *)block + off;
                void (*fn)(void *, void *) = rec->fn;
                fn(base, self);
            } else {
                free(self);
            }
        }
    }
}

}
