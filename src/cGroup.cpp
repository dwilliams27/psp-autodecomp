class cBase;
class cFile;
class cMemPool;
class cType;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void End(void);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

struct cGroupNode {
    char _pad0[0x28];
    unsigned short flags;
    char _pad2[6];
    cHandle handle;
    cGroupNode *next;
};

class cGroup {
public:
    char _pad[0xC];
    cGroupNode *mList;

    const cType *GetType(void) const;
    void Write(cFile &) const;
};

class cListSubscriber {
public:
    const cType *GetType(void) const;
};

extern cType *D_000385DC;
extern cType *D_00038880;
extern cType *D_00040C94;

const cType *cGroup::GetType(void) const {
    if (D_00040C94 == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType((const char *)0x36C830,
                                               (const char *)0x36C838,
                                               1, 0, 0, 0, 0, 0);
        }
        D_00040C94 = cType::InitializeType(0, 0, 4, D_000385DC,
                                           0, 0, 0, 0);
    }
    return D_00040C94;
}

void cGroup::Write(cFile &file) const {
    register void *temp_s1 __asm__("$17");
    void *var_a0;
    void *var_s1;
    cWriteBlock wb(file, 2);
    var_s1 = *(void **)((char *)this + 0xC);
    if (var_s1 != 0) {
        do {
            int write = *(unsigned short *)((char *)var_s1 + 0x28) & 4;
            write = write != 0;
            write = (unsigned char)write;
            if (write != 0) {
                wb.Write(true);
                ((cHandle *)((char *)var_s1 + 0x30))->Write(wb);
            }
            temp_s1 = *(void **)((char *)var_s1 + 0x34);
            if (temp_s1 != *(void **)((char *)this + 0xC)) {
                var_a0 = temp_s1;
            } else {
                var_a0 = 0;
            }
            var_s1 = var_a0;
        } while (var_s1 != 0);
    }
    wb.Write(false);
    wb.End();
}

const cType *cListSubscriber::GetType(void) const {
    if (D_00038880 == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType((const char *)0x36C830,
                                               (const char *)0x36C838,
                                               1, 0, 0, 0, 0, 0);
        }
        D_00038880 = cType::InitializeType(0, 0, 0x187, D_000385DC,
                                           0, 0, 0, 0);
    }
    return D_00038880;
}
