class cBase;
class cFile;
class cMemPool;
class cType;
class gcDesiredTableTemplate;
class gcDesiredTableTemplateHelper;
class gcEvent;
class gcValue;

template <class T>
class cBaseArrayT {
public:
    void *mData;
    int mSize;
};

class gcTableTemplate {
public:
    void Query(const cBaseArrayT<gcValue *> &, cBaseArrayT<gcValue *>,
               const gcEvent &, cBaseArrayT<gcValue *>, cBaseArrayT<gcValue *>);
};

template <class T, class H, class O>
class gcDesiredObjectT {
public:
    H *Get(bool) const;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    cFile *_file;
    int _pos;
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cBaseArray {
public:
    void Write(cWriteBlock &) const;
};

class gcAction {
public:
    void Write(cFile &) const;
};

class gcDoTableQuery : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void GetText(char *) const;
    void Write(cFile &) const;
    float Evaluate(void) const;
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

typedef void (*WriteFn)(cBase *, cFile *);

struct TypeMethod {
    short offset;
    short pad;
    WriteFn fn;
};

struct GetTextSlot {
    short offset;
    short pad;
    void (*fn)(void *, char *);
};

void gcAction_gcAction(gcDoTableQuery *, cBase *);
void gcDesiredObject_gcDesiredObject(void *, cBase *);
void gcEvent_gcEvent(void *, cBase *, const char *);
void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);

extern char gcActionvirtualtable[];
extern char D_00000838[];
extern const char gcDoTableQuery_base_name[] asm("D_0036D894");
extern const char gcDoTableQuery_base_desc[] asm("D_0036D89C");

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoTableQuery asm("D_0009F714");

cBase *gcDoTableQuery::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoTableQuery *result = 0;
    gcDoTableQuery *obj =
        (gcDoTableQuery *)entry->fn(base, 0x84, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcActionvirtualtable;
        gcDesiredObject_gcDesiredObject((char *)obj + 0x0C, (cBase *)obj);
        ((void **)obj)[4] = D_00000838;
        ((int *)obj)[6] = 1;
        ((int *)obj)[7] = 0;
        ((int *)obj)[4] = 0x38A000;
        ((int *)obj)[8] = 0;
        ((gcDoTableQuery **)obj)[9] = obj;
        ((int *)obj)[10] = 0;
        ((gcDoTableQuery **)obj)[11] = obj;
        gcEvent_gcEvent((char *)obj + 0x30, (cBase *)obj, 0);
        gcEvent_gcEvent((char *)obj + 0x4C, (cBase *)obj, 0);
        gcEvent_gcEvent((char *)obj + 0x68, (cBase *)obj, 0);
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDoTableQuery::GetType(void) const {
    if (!type_gcDoTableQuery) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoTableQuery_base_name, gcDoTableQuery_base_desc, 1,
                        0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoTableQuery = cType::InitializeType(
            0, 0, 0x21A, type_action, gcDoTableQuery::New, 0, 0, 0);
    }
    return type_gcDoTableQuery;
}

void gcDoTableQuery::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    gcAction::Write(file);

    TypeMethod *slot0 = (TypeMethod *)((char *)*(void **)((char *)this + 0x10) + 0x28);
    cBase *base0 = (cBase *)((char *)this + 0x0C);
    slot0->fn((cBase *)((char *)base0 + slot0->offset), wb._file);
    ((const cBaseArray *)((const char *)this + 0x20))->Write(wb);
    ((const cBaseArray *)((const char *)this + 0x28))->Write(wb);

    TypeMethod *slot1 = (TypeMethod *)((char *)*(void **)((char *)this + 0x34) + 0x28);
    cBase *base1 = (cBase *)((char *)this + 0x30);
    slot1->fn((cBase *)((char *)base1 + slot1->offset), wb._file);
    TypeMethod *slot2 = (TypeMethod *)((char *)*(void **)((char *)this + 0x50) + 0x28);
    cBase *base2 = (cBase *)((char *)this + 0x4C);
    slot2->fn((cBase *)((char *)base2 + slot2->offset), wb._file);
    TypeMethod *slot3 = (TypeMethod *)((char *)*(void **)((char *)this + 0x6C) + 0x28);
    cBase *base3 = (cBase *)((char *)this + 0x68);
    slot3->fn((cBase *)((char *)base3 + slot3->offset), wb._file);
    wb.End();
}

float gcDoTableQuery::Evaluate(void) const {
    gcDesiredTableTemplateHelper *table =
        ((const gcDesiredObjectT<gcDesiredTableTemplate, gcDesiredTableTemplateHelper, gcTableTemplate> *)((const char *)this + 0x0C))->Get(true);
    float result = 1.0f;
    if (table != 0) {
        ((gcTableTemplate *)table)->Query(
            *(const cBaseArrayT<gcValue *> *)((const char *)this + 0x20),
            *(cBaseArrayT<gcValue *> *)((const char *)this + 0x28),
            *(const gcEvent *)((const char *)this + 0x30),
            *(cBaseArrayT<gcValue *> *)((const char *)this + 0x4C),
            *(cBaseArrayT<gcValue *> *)((const char *)this + 0x68));
    }
    return result;
}

void gcDoTableQuery::GetText(char *buf) const {
    register const gcDoTableQuery *self __asm__("$16") = this;
    register char *out __asm__("$17") = buf;
    char local0[256];
    char local1[256];

    local0[0] = '\0';
    local1[0] = '\0';

    cStrAppend(out, (const char *)0x36EF8C);

    char *typeInfo0 = *(char **)((const char *)self + 0x10);
    GetTextSlot *slot0 = (GetTextSlot *)(typeInfo0 + 0x78);
    char *sub0 = (char *)self + 0x0C;
    slot0->fn(sub0 + slot0->offset, out);

    int countOk = 0;
    int *values = *(int **)((const char *)self + 0x20);
    if (values != 0) {
        countOk = values[-1] > 0;
    }

    if (countOk != 0) {
        cStrAppend(out, (const char *)0x36EF9C);
        __asm__ volatile("" ::: "memory");

        register int index __asm__("$18") = 0;
        register const char *comma __asm__("$19") = (const char *)0x36DAD8;
        register int offset __asm__("$20") = 0;
        register const char *none __asm__("$21") = (const char *)0x36DACC;
        register const char *middle __asm__("$22") = (const char *)0x36EFA4;
        register const char *close __asm__("$23") = (const char *)0x36EBE4;
        __asm__ volatile("" : "+r"(offset));

        while (1) {
            int count = 0;
            if (*(int **)((const char *)self + 0x20) != 0) {
                count = (*(int **)((const char *)self + 0x20))[-1];
            }
            if (index >= count) {
                break;
            }

            int *array = *(int **)((const char *)self + 0x20);
            int *entry = (int *)((char *)array + offset);
            if (index > 0) {
                cStrAppend(out, comma);
                array = *(int **)((const char *)self + 0x20);
                entry = (int *)((char *)array + offset);
            }

            int obj = *entry;
            if (obj != 0) {
                char *type = *(char **)(obj + 4);
                GetTextSlot *slot = (GetTextSlot *)(type + 0xC8);
                slot->fn((char *)obj + slot->offset, out);
            } else {
                cStrCat(out, none);
            }
            cStrCat(out, middle);

            int *targets = *(int **)((const char *)self + 0x28);
            int target = *(int *)((char *)targets + offset);
            if (target != 0) {
                char *type = *(char **)(target + 4);
                GetTextSlot *slot = (GetTextSlot *)(type + 0xC8);
                slot->fn((char *)target + slot->offset, out);
            } else {
                cStrCat(out, none);
            }
            cStrCat(out, close);

            index += 1;
            offset += 4;
        }
    }

    int valid = *(int *)((const char *)self + 0x38) == 0;
    valid &= 0xFF;
    if (valid == 0) {
        cStrAppend(out, (const char *)0x36EFA8);
    }

    valid = *(int *)((const char *)self + 0x54) == 0;
    valid &= 0xFF;
    if (valid == 0) {
        cStrAppend(out, (const char *)0x36EFB4);
    }

    valid = *(int *)((const char *)self + 0x70) == 0;
    valid &= 0xFF;
    if (valid == 0) {
        cStrAppend(out, (const char *)0x36EFC0);
    }

    cStrAppend(out, (const char *)0x36DCEC);
}
