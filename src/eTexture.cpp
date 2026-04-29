class cBase;
class cFile;

class cObject {
public:
    void Write(cFile &) const;
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
