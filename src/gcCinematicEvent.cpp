class cBase;
class cMemPool;
class cType;

inline void *operator new(unsigned int, void *p) { return p; }

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcCinematicEvent {
public:
    gcCinematicEvent(cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

extern char gcCinematicEventclassdesc[];
extern const char gcCinematicEvent_event_name[];
extern const char gcCinematicEvent_base_name[];
extern const char gcCinematicEvent_base_desc[];

extern cType *D_000385DC;
extern cType *D_00099AC4;

void gcEvent_ctor(void *, cBase *, const char *);

cBase *gcCinematicEvent::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcCinematicEvent *result = 0;
    gcCinematicEvent *obj = (gcCinematicEvent *)entry->fn(base, 0x50, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcCinematicEvent(parent);
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcCinematicEvent::GetType(void) const {
    if (D_00099AC4 == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType(gcCinematicEvent_base_name,
                                               gcCinematicEvent_base_desc,
                                               1, 0, 0, 0, 0, 0);
        }
        D_00099AC4 = cType::InitializeType(0, 0, 0x164, D_000385DC,
                                           &gcCinematicEvent::New, 0, 0, 0);
    }
    return D_00099AC4;
}

gcCinematicEvent::gcCinematicEvent(cBase *parent) {
    ((cBase **)this)[0] = parent;
    ((void **)this)[1] = gcCinematicEventclassdesc;
    ((int *)this)[2] = 0;
    ((int *)this)[3] = 0;
    gcEvent_ctor((char *)this + 0x10, (cBase *)this, gcCinematicEvent_event_name);
    ((int *)this)[0x2C / 4] = 0;
    ((int *)this)[0x30 / 4] = 0;
    ((int *)this)[0x34 / 4] = 0;
    ((int *)this)[0x38 / 4] = 0;
    ((int *)this)[0x3C / 4] = 0;
    ((int *)this)[0x40 / 4] = 0;
    ((int *)this)[0x44 / 4] = (int)this | 1;
    ((char *)this)[0x48] = 0;
    ((char *)this)[0x49] = 0;
    ((int *)this)[0x4C / 4] = 0;
}
