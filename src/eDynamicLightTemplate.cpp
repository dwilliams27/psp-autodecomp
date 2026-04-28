// eDynamicLightTemplate.cpp - decompiled from eAll_psp.obj
//
// Functions:
//   0x0007744c  eDynamicLightTemplate::Write(cFile &) const
//   0x00077608  eDynamicLightTemplate::eDynamicLightTemplate(cBase *)
//   0x0020fe94  eDynamicLightTemplate::New(cMemPool *, cBase *) static

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(float);
    void End(void);
};

class cObject {
public:
    cObject(cBase *);
};

class eDynamicGeomTemplate {
public:
    void Write(cFile &) const;
};

class cHandle {
public:
    int mIndex;
    void Write(cWriteBlock &) const;
};

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class eDynamicLightTemplate : public cObject {
public:
    eDynamicLightTemplate(cBase *);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

extern "C" {
    void eDynamicLightTemplate__eDynamicLightTemplate_cBaseptr(void *self, cBase *parent);
}

// -- eDynamicLightTemplate::Write(cFile &) const @ 0x0007744c --
#pragma control sched=1
void eDynamicLightTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const eDynamicGeomTemplate *)this)->Write(file);

    cHandle *handles0 = *(cHandle **)((const char *)this + 0x48);
    int count0 = 0;
    if (handles0 != 0) {
        count0 = *((int *)handles0 - 1) & 0x3FFFFFFF;
    }
    wb.Write(count0);

    cHandle *handles = *(cHandle **)((const char *)this + 0x48);
    int count = 0;
    if (handles != 0) {
        count = *((int *)handles - 1) & 0x3FFFFFFF;
    }
    int offset = 0;
    int i = 0;
    if (i < count) {
        cHandle *handle = handles + offset;
        do {
            handle->Write(wb);
            i++;
            handle++;
        } while (i < count);
    }

    wb.Write(*(const float *)((const char *)this + 0x4C));
    wb.End();
}
#pragma control sched=2

// -- eDynamicLightTemplate::eDynamicLightTemplate(cBase *) @ 0x00077608 --
#pragma control sched=1
eDynamicLightTemplate::eDynamicLightTemplate(cBase *parent) : cObject(parent) {
    *(void **)((char *)this + 4) = (void *)0x00384278;
    *(void **)((char *)this + 0x48) = 0;
    *(float *)((char *)this + 0x4C) = 1.0f;
    __asm__ volatile("" ::: "memory");
    *(float *)((char *)this + 0x44) = 50.0f;
}
#pragma control sched=2

// -- eDynamicLightTemplate::New(cMemPool *, cBase *) static @ 0x0020fe94 --
#pragma control sched=1
cBase *eDynamicLightTemplate::New(cMemPool *pool, cBase *parent) {
    eDynamicLightTemplate *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eDynamicLightTemplate *obj = (eDynamicLightTemplate *)rec->fn(base, 0x50, 4, 0, 0);
    if (obj != 0) {
        eDynamicLightTemplate__eDynamicLightTemplate_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
#pragma control sched=2
