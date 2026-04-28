#include "cBase.h"

class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(bool);
    void End(void);
};

struct gcDesiredUIWidgetHelper {
    int _a;
    int _b;
    int _c;
    void Write(cWriteBlock &) const;
    void GetText(char *) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
};

class gcDoUISetOrigin {
public:
    void GetText(char *) const;
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
    static cBase *New(cMemPool *, cBase *);
};

void cStrAppend(char *, const char *, ...);
void gcAction_Write(const gcDoUISetOrigin *, cFile &);
gcDoUISetOrigin *dcast(const cBase *);
void gcAction__gcAction_cBaseptr__0012F4C8(void *, cBase *);
void gcDesiredUIWidgetHelper_ctor(void *, int);

extern char gcDoUISetOriginvirtualtable[];
extern const char gcDoUISetOrigin_fmt[];
extern const char gcDoUISetOrigin_str2[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

// 0x0030fca4, 160B
cBase *gcDoUISetOrigin::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoUISetOrigin *result = 0;
    gcDoUISetOrigin *obj = (gcDoUISetOrigin *)entry->fn(base, 0x20, 4, 0, 0);
    if (obj != 0) {
        gcAction__gcAction_cBaseptr__0012F4C8(obj, parent);
        ((void **)obj)[1] = gcDoUISetOriginvirtualtable;
        gcDesiredUIWidgetHelper_ctor((char *)obj + 0xC, 1);
        *(int *)((char *)obj + 0x18) = 5;
        *(unsigned char *)((char *)obj + 0x1C) = 0;
        result = obj;
    }
    return (cBase *)result;
}

// 0x0030fc24, 128B
void gcDoUISetOrigin::AssignCopy(const cBase *other) {
    gcDoUISetOrigin *src = dcast(other);
    int *self_i = (int *)this;
    int *src_i = (int *)src;
    int v = self_i[2] & ~3;
    self_i[2] = v;
    self_i[2] = v | (src_i[2] & 3);
    self_i[3] = src_i[3];
    int *s1 = (int *)((char *)src + 0x10);
    int *d1 = (int *)((char *)this + 0x10);
    int *s2 = (int *)((char *)src + 0x14);
    int *d2 = (int *)((char *)this + 0x14);
    *d1 = *s1;
    *d2 = *s2;
    self_i[6] = src_i[6];
    *(unsigned char *)((char *)this + 0x1C) = *(const unsigned char *)((char *)src + 0x1C);
}

// 0x0030fe5c, 112B
void gcDoUISetOrigin::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction_Write(this, file);
    ((gcDesiredUIWidgetHelper *)((char *)this + 0xC))->Write(wb);
    wb.Write(*(int *)((char *)this + 0x18));
    wb.Write(*(bool *)((char *)this + 0x1C));
    wb.End();
}

// 0x00310110, 80B
void gcDoUISetOrigin::GetText(char *buf) const {
    char local[256];
    local[0] = *local = '\0';
    ((gcDesiredUIWidgetHelper *)((char *)this + 0xC))->GetText(local);
    cStrAppend(buf, gcDoUISetOrigin_fmt, local, gcDoUISetOrigin_str2);
}

// 0x00310160, 128B
void gcDoUISetOrigin::VisitReferences(unsigned int flags, cBase *ctx, void (*cb)(cBase *, unsigned int, void *), void *user, unsigned int mask) {
    if (cb != 0) {
        cb(ctx, (unsigned int)(void *)this, user);
    }
    ((gcDesiredUIWidgetHelper *)((char *)this + 0xC))->VisitReferences(flags, (cBase *)this, cb, user, mask);
}
