// Days of Thunder decompilation: gcGameStrings methods

class cFile;
class cMemPool;

class cBase {
public:
    void *_b0;
    void *_b4;
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class gcStringValue : public cBase {
public:
    void Write(cFile &) const;
};

class gcGameStrings : public gcStringValue {
public:
    int mValue;  // offset 8

    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    void GetName(char *) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
};

gcGameStrings *dcast(const cBase *);
void cStrAppend(char *, const char *, ...);

extern const char gcGameStrings_fmt[];
extern const char gcGameStrings_name[];
extern char gcGameStrings_cBase_vtable[];

void *cMemPool_GetPoolFromPtr(void *);

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

// ============================================================
// gcGameStrings::AssignCopy(const cBase *)
// ============================================================
void gcGameStrings::AssignCopy(const cBase *base) {
    gcGameStrings *src = dcast(base);
    this->mValue = src->mValue;
}

// ============================================================
// gcGameStrings::Write(cFile &) const
// ============================================================
void gcGameStrings::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    this->gcStringValue::Write(file);
    wb.Write(this->mValue);
    wb.End();
}

// ============================================================
// gcGameStrings::GetName(char *) const
// ============================================================
void gcGameStrings::GetName(char *buf) const {
    cStrAppend(buf, gcGameStrings_fmt, gcGameStrings_name);
}

// ============================================================
// gcGameStrings::VisitReferences
// ============================================================
void gcGameStrings::VisitReferences(unsigned int flags, cBase *ctx,
                                    void (*cb)(cBase *, unsigned int, void *),
                                    void *user, unsigned int mask) {
    if (cb != 0) {
        cb(ctx, (unsigned int)(void *)this, user);
    }
}

// ============================================================
// gcGameStrings::~gcGameStrings(void)
// ============================================================
extern "C" {

void gcGameStrings___dtor_gcGameStrings_void(gcGameStrings *self, int flags) {
    if (self != 0) {
        *(void **)((char *)self + 4) = gcGameStrings_cBase_vtable;
        if (flags & 1) {
            void *pool = cMemPool_GetPoolFromPtr(self);
            void *block = *(void **)((char *)pool + 0x24);
            DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
            short off = rec->offset;
            rec->fn((char *)block + off, self);
        }
    }
}

}
