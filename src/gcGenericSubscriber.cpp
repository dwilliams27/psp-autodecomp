// ODR-WARNING: local class redeclarations keep this TU from changing shared headers.
class cBase;
class cFile;
class cMemPool;
class cType;

class gcGenericSubscriber {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Attach(void);
    void Detach(void);
    int GetItem(int) const;
    int GetIndex(void *) const;
    int IsModifiable(void) const;
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *, cBase *(*)(cMemPool *, cBase *), const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cListSubscriber {
public:
    cListSubscriber(cBase *);
    void Attach(void);
    void Detach(void);
    void Write(cFile &) const;
};

class cSubscriberArray {
public:
    int *mData;
    cSubscriberArray &operator=(const cSubscriberArray &);
};

class gcUIDialog;

class gcUI {
public:
    static gcUIDialog *GetActiveSpawnedDialog(const gcUIDialog *);
};

gcGenericSubscriber *dcast(const cBase *);

extern char gcGenericSubscribervirtualtable[];
extern const char gcGenericSubscriber_cBase_name[];
extern const char gcGenericSubscriber_cBase_desc[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct gcUI_ActiveSpawnScan {
    char pad[0x54];
    void **active;
};

extern "C" void cListSubscriber_cListSubscriber(void *, cBase *);

cBase *gcGenericSubscriber::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcGenericSubscriber *result = 0;
    gcGenericSubscriber *obj = (gcGenericSubscriber *)entry->fn(base, 0x28, 4, 0, 0);
    if (obj != 0) {
        cListSubscriber_cListSubscriber(obj, parent);
        *(void **)((char *)obj + 4) = gcGenericSubscribervirtualtable;
        *(int *)((char *)obj + 0x24) = 0;
        result = obj;
    }
    return (cBase *)result;
}

static cType *type_cBase;
static cType *type_cListSubscriber;
static cType *type_gcGenericSubscriber;

const cType *gcGenericSubscriber::GetType(void) const {
    if (!type_gcGenericSubscriber) {
        if (!type_cListSubscriber) {
            if (!type_cBase) {
                type_cBase = cType::InitializeType(
                    gcGenericSubscriber_cBase_name, gcGenericSubscriber_cBase_desc,
                    1, 0, 0, 0, 0, 0);
            }
            type_cListSubscriber = cType::InitializeType(
                0, 0, 0x187, type_cBase, 0, 0, 0, 0);
        }
        type_gcGenericSubscriber = cType::InitializeType(
            0, 0, 0x203, type_cListSubscriber, gcGenericSubscriber::New, 0, 0, 0);
    }
    return type_gcGenericSubscriber;
}

void gcGenericSubscriber::Attach(void) {
    ((cListSubscriber *)this)->Attach();
}

void gcGenericSubscriber::Detach(void) {
    ((cListSubscriber *)this)->Detach();
}

int gcGenericSubscriber::GetItem(int idx) const {
    int *arr = *(int **)((const char *)this + 0x24);
    int count = 0;
    if (arr) count = arr[-1] & 0x3FFFFFFF;
    if (count <= 0) return idx + 1;
    int result = 0;
    if (idx < 0) return result;
    if (idx >= count) return result;
    result = arr[idx];
    return result;
}

int gcGenericSubscriber::GetIndex(void *p) const {
    float *arr = *(float **)((const char *)this + 0x24);
    int count = 0;
    if (arr) {
        count = ((int *)arr)[-1] & 0x3FFFFFFF;
    }
    if (count > 0) {
        int limit = *(int *)((const char *)this + 0x1C);
        float value = *(float *)&p;
        int i = 0;
        if (i < limit) {
            do {
                if (*arr == value) {
                    return i;
                }
                i++;
                arr++;
            } while (i < limit);
        }
        return -1;
    }
    return (int)p - 1;
}

gcUIDialog *gcUI::GetActiveSpawnedDialog(const gcUIDialog *dialog) {
    gcUI *ui = *(gcUI **)0x37D7BC;
    if (ui == 0) {
        return (gcUIDialog *)dialog;
    }
    if (dialog == 0) {
        return 0;
    }

    gcUIDialog *active = *(gcUIDialog **)0x37D7C0;
    if (active != 0) {
        if (*(gcUIDialog **)active == dialog) {
            return active;
        }
    }
    int i = *(int *)((char *)ui + 0xD4);
    if (i >= 0) {
        gcUI_ActiveSpawnScan *slot = (gcUI_ActiveSpawnScan *)((char *)ui + i * 4);
        do {
            active = (gcUIDialog *)slot->active;
            if (active != 0) {
                if (*(gcUIDialog **)active == dialog) {
                    return active;
                }
            }
            i--;
            slot = (gcUI_ActiveSpawnScan *)((char *)slot - 4);
        } while (i >= 0);
    }
    return (gcUIDialog *)dialog;
}

void gcGenericSubscriber::AssignCopy(const cBase *base) {
    struct Layout {
        char _p0[0x08];
        int f08;
        int f0C;
        unsigned char f10;
        char _p2[3];
        int f14;
        int f18;
        int f1C;
        int f20;
    };
    gcGenericSubscriber *other = dcast(base);
    Layout *self_l = (Layout *)this;
    Layout *other_l = (Layout *)other;
    self_l->f08 = other_l->f08;
    self_l->f0C = other_l->f0C;
    self_l->f10 = other_l->f10;
    self_l->f14 = other_l->f14;
    self_l->f18 = other_l->f18;
    self_l->f1C = other_l->f1C;
    self_l->f20 = other_l->f20;
    ((cSubscriberArray *)((char *)this + 0x24))->operator=(*(cSubscriberArray *)((char *)other + 0x24));
}

void gcGenericSubscriber::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const cListSubscriber *)this)->Write(file);
    int count = 0;
    int *arr = *(int **)((const char *)this + 0x24);
    if (arr) count = arr[-1] & 0x3FFFFFFF;
    wb.Write(count);
    wb.End();
}
