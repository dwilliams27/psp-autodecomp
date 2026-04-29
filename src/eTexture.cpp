class cBase;
class cFile;
class cMemPool;

class cType;

class cObject {
public:
    cObject(cBase *);
    void Write(cFile &) const;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cWriteBlock {
public:
    cFile *_file;
    int _pos;

    cWriteBlock(cFile &, unsigned int);
    void Write(char);
    void Write(short);
    void Write(float);
    void Write(int, const float *);
    void End(void);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

class eTexture : public cObject {
public:
    eTexture(cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

class eTextureFilter {
public:
    void Write(cFile &) const;
};

class eWaterFilter : public eTextureFilter {
public:
    void Write(cFile &) const;
};

class gcAction {
public:
    void Write(cFile &) const;
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

struct TypeDispatchEntry {
    short offset;
    short pad;
    void (*fn)(void *, cFile *);
};

class gcDoEntityBipedSetShape : public gcAction {
public:
    void Write(cFile &) const;
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00040FE8;

eTexture::eTexture(cBase *parent) : cObject(parent) {
    *(void **)((char *)this + 4) = (void *)0x37FD48;
    *(unsigned char *)((char *)this + 0x44) = 1;
    *(unsigned char *)((char *)this + 0x45) = 0;
    *(unsigned char *)((char *)this + 0x46) = 1;
    *(unsigned char *)((char *)this + 0x47) = 0;
    *(short *)((char *)this + 0x48) = 0;
    *(short *)((char *)this + 0x4A) = 0;
}

const cType *eTexture::GetType(void) const {
    if (D_00040FE8 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                       (const char *)0x36CD7C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                                   &cNamed::New, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                               0, 0, 0, 0);
        }
        D_00040FE8 = cType::InitializeType(0, 0, 0xA, D_000385E4,
                                           0, (const char *)0x36CDA8,
                                           (const char *)0x36CDB4, 5);
    }
    return D_00040FE8;
}

void eTexture::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const cObject *)this)->Write(file);
    wb.Write(*(const char *)((const char *)this + 0x44));
    wb.Write(*(const char *)((const char *)this + 0x45));
    wb.Write(*(const char *)((const char *)this + 0x46));
    wb.Write(*(const short *)((const char *)this + 0x48));
    wb.Write(*(const short *)((const char *)this + 0x4A));
    wb.End();
}

#pragma control sched=1
void eWaterFilter::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    eTextureFilter::Write(file);
    ((const cHandle *)((const char *)this + 0x10))->Write(wb);
    ((const cHandle *)((const char *)this + 0x14))->Write(wb);
    wb.Write(*(const float *)((const char *)this + 0x18));
    wb.Write(*(const float *)((const char *)this + 0x1C));
    wb.Write(*(const float *)((const char *)this + 0x20));
    wb.End();
}
#pragma control sched=2

void gcDoEntityBipedSetShape::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    gcAction::Write(file);

    char *typeInfo = *(char **)((const char *)this + 0x10);
    TypeDispatchEntry *entry = (TypeDispatchEntry *)(typeInfo + 0x28);
    char *base = (char *)this + 0x0C;
    entry->fn(base + entry->offset, wb._file);

    wb.Write(3, (const float *)((const char *)this + 0x40));
    ((const gcDesiredValue *)((const char *)this + 0x38))->Write(wb);
    wb.End();
}
