class cBase;
class cFile;

extern "C" {
    void cStrCat(char *, const char *);
    void *cMemPool_GetPoolFromPtr(void *);
    void free(void *);
}

class gcExpression {
public:
    void GetTextRecursive(char *) const;
    void Write(cFile &) const;
};

class gcAction : public gcExpression {
public:
    cBase *mOwner;
    void *mVTable;
    unsigned int mNext;

    gcAction(cBase *);
    unsigned int GetTextColor(void) const;
    void GetTextRecursive(char *) const;
    void Write(cFile &) const;
};

extern char gcActionvirtualtable[];
extern const char gcAction_spaceStr[];

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

gcAction::gcAction(cBase *b) {
    mOwner = b;
    mVTable = gcActionvirtualtable;
    mNext = 0;
}

unsigned int gcAction::GetTextColor(void) const {
    volatile unsigned int hl;
    volatile unsigned int def;
    int flag = 0;
    if (mNext & 1) {
        flag = 1;
    }
    if (flag) {
        hl = 0xFF808080;
        return hl;
    }
    def = 0xFF000000;
    unsigned int result;
    __asm__ volatile(
        "addiu $v0, $sp, 4\n\t"
        "lw %0, 0($v0)"
        : "=&v"(result)
    );
    return result;
}

void gcAction::GetTextRecursive(char *out) const {
    int flag = 0;
    if (mNext & 1) {
        flag = 1;
    }
    if (flag) {
        cStrCat(out, gcAction_spaceStr);
    }
    gcExpression::GetTextRecursive(out);
}

void gcAction::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    gcExpression::Write(file);
    int flag = 0;
    if (mNext & 1) {
        flag = 1;
    }
    wb.Write(flag);
    wb.End();
}

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

extern "C" void gcAction___dtor_gcAction_void(gcAction *self, int flags) {
    if (self != 0) {
        self->mVTable = gcActionvirtualtable;
        if (flags & 1) {
            void *pool = cMemPool_GetPoolFromPtr(self);
            if (pool != 0) {
                void *block = *(void **)((char *)pool + 0x24);
                DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
                short off = rec->offset;
                rec->fn((char *)block + off, self);
            } else {
                free(self);
            }
        }
    }
}
