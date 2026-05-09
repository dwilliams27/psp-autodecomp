// gcValLobbyScoreboardInfo -- decompiled from gcAll_psp.obj

class cBase;
class cFile;
class cFileHandle;
class cMemPool;
class cType;

struct DispatchEntry {
    short offset;
    short pad;
    cType *(*fn)(void *);
};

struct GetTextSlot {
    short offset;
    short pad;
    void *fn;
};

class cType {
public:
    char pad[0x1C];
    cType *mParent;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cFile {
public:
    void SetCurrentPos(unsigned int);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];

    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void WriteBase(const cBase *);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];

    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
    void ReadBase(cMemPool *, cBase *, cBase *&);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
};

class gcValue {
public:
    cBase *mParent;
    void *mVtable;

    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcValLobbyScoreboardInfo : public gcValue {
public:
    int mField8;
    int mFieldC;
    int mDesired10;
    int mDesired14;
    int mDesired18;
    int mDesired1C;
    int mBase20;
    int mBase24;

    gcValLobbyScoreboardInfo &operator=(const gcValLobbyScoreboardInfo &);
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    void GetText(char *) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcValRandomNumber : public gcValue {
public:
    int mField8;
    int mDesiredC;
    int mDesired10;
    char pad14;

    int Read(cFile &, cMemPool *);
};

void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValLobbyScoreboardInfo;

// 0x0034af2c -- gcValLobbyScoreboardInfo::AssignCopy(const cBase *)
void gcValLobbyScoreboardInfo::AssignCopy(const cBase *base) {
    const gcValLobbyScoreboardInfo *other = 0;

    if (base != 0) {
        if (!type_gcValLobbyScoreboardInfo) {
            if (!type_value) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            (const char *)0x36D894, (const char *)0x36D89C,
                            1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_value = cType::InitializeType(
                    0, 0, 0x6C, type_expression, 0, 0, 0, 0x80);
            }
            type_gcValLobbyScoreboardInfo = cType::InitializeType(
                0, 0, 0x124, type_value, gcValLobbyScoreboardInfo::New,
                0, 0, 0);
        }

        DispatchEntry *entry =
            (DispatchEntry *)((char *)*(void **)((char *)base + 4) + 8);
        cType *wanted = type_gcValLobbyScoreboardInfo;
        cType *type = entry->fn((char *)base + entry->offset);
        int ok;

        if (wanted == 0) {
            ok = 0;
        } else if (type != 0) {
        loop:
            if (type == wanted) {
                ok = 1;
            } else {
                type = type->mParent;
                if (type != 0) {
                    goto loop;
                }
                goto fail;
            }
        } else {
fail:
            ok = 0;
        }
        if (ok != 0) {
            other = (const gcValLobbyScoreboardInfo *)base;
        }
    }

    operator=(*other);
}

// 0x0034b290 -- gcValLobbyScoreboardInfo::Write(cFile &) const
void gcValLobbyScoreboardInfo::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcValue *)this)->Write(file);
    wb.Write(mField8);
    wb.Write(mFieldC);
    ((const gcDesiredValue *)((const char *)this + 0x10))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x18))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x14))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x1C))->Write(wb);

    int val0 = mBase20;
    int flag0 = 0;
    if (val0 & 1) {
        flag0 = 1;
    }
    cBase *ptr0;
    if (flag0 != 0) {
        ptr0 = 0;
    } else {
        ptr0 = (cBase *)val0;
    }
    wb.WriteBase(ptr0);

    int val1 = mBase24;
    int flag1 = 0;
    if (val1 & 1) {
        flag1 = 1;
    }
    cBase *ptr1;
    if (flag1 != 0) {
        ptr1 = 0;
    } else {
        ptr1 = (cBase *)val1;
    }
    wb.WriteBase(ptr1);
    wb.End();
}

// 0x0034b384 -- gcValLobbyScoreboardInfo::Read(cFile &, cMemPool *)
int gcValLobbyScoreboardInfo::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 1, true);

    if (rb[3] != 1 || gcValue::Read(file, pool) == 0) {
        ((cFile *)rb[0])->SetCurrentPos(rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x08, 4);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x0C, 4);
    ((gcDesiredValue *)((char *)this + 0x10))->Read(*(cReadBlock *)rb);
    ((gcDesiredValue *)((char *)this + 0x18))->Read(*(cReadBlock *)rb);
    ((gcDesiredValue *)((char *)this + 0x14))->Read(*(cReadBlock *)rb);
    ((gcDesiredValue *)((char *)this + 0x1C))->Read(*(cReadBlock *)rb);

    int sp14;
    int sp18;
    int value = *(int *)((char *)this + 0x20);
    int firstTag = value & 1;
    int firstFlag = 0;
    if (firstTag != 0) {
        firstFlag = 1;
    }

    int outValue;
    if (firstFlag != 0) {
        outValue = 0;
        goto first_out_done;
    }
    outValue = value;
first_out_done:
    sp14 = outValue;

    int firstFlag2 = 0;
    if (firstTag != 0) {
        firstFlag2 = 1;
    }

    int base;
    if (firstFlag2 != 0) {
        base = value & ~1;
    } else {
        base = *(int *)value;
    }

    cMemPool *childPool = cMemPool::GetPoolFromPtr((char *)this + 0x20);
    ((cReadBlock *)rb)->ReadBase(childPool, (cBase *)base, *(cBase **)&sp14);

    int value2 = *(int *)((char *)this + 0x24);
    int newValue = sp14;
    if (newValue != 0) {
    } else {
        newValue = base | 1;
    }
    int secondTag = value2 & 1;
    *(int *)((char *)this + 0x20) = newValue;

    int secondFlag = 0;
    if (secondTag != 0) {
        secondFlag = 1;
    }

    if (secondFlag != 0) {
        outValue = 0;
        goto second_out_done;
    }
    outValue = value2;
second_out_done:
    sp18 = outValue;

    int secondFlag2 = 0;
    if (secondTag != 0) {
        secondFlag2 = 1;
    }

    if (secondFlag2 != 0) {
        base = value2 & ~1;
    } else {
        base = *(int *)value2;
    }

    childPool = cMemPool::GetPoolFromPtr((char *)this + 0x24);
    ((cReadBlock *)rb)->ReadBase(childPool, (cBase *)base, *(cBase **)&sp18);

    int newValue2 = sp18;
    if (newValue2 != 0) {
    } else {
        newValue2 = base | 1;
    }
    *(int *)((char *)this + 0x24) = newValue2;

    __0oKcReadBlockdtv(rb, 2);
    return result;
}

// 0x00357f78 -- gcValRandomNumber::Read(cFile &, cMemPool *)
int gcValRandomNumber::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    char sp14;

    __0oKcReadBlockctR6FcFileUib(rb, file, 1, true);
    if (rb[3] != 1 || gcValue::Read(file, pool) == 0) {
        ((cFile *)rb[0])->SetCurrentPos(rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x08, 4);
    cFileSystem::Read(*(cFileHandle **)rb[0], &sp14, 1);
    *(char *)((char *)this + 0x14) = sp14 != 0;
    ((gcDesiredValue *)((char *)this + 0x0C))->Read(*(cReadBlock *)rb);
    ((gcDesiredValue *)((char *)this + 0x10))->Read(*(cReadBlock *)rb);

    __0oKcReadBlockdtv(rb, 2);
    return result;
}

// 0x0034bc6c -- gcValLobbyScoreboardInfo::GetText(char *) const
void gcValLobbyScoreboardInfo::GetText(char *buf) const {
    register const gcValLobbyScoreboardInfo *self __asm__("$16") = this;
    register char *out __asm__("$17") = buf;
    char local0[256];
    char local1[256];

    cStrAppend(out, (const char *)0x36F520);

    register const char *close __asm__("$18") = (const char *)0x36E2E8;
    if (*(int *)((const char *)self + 0x0C) == 0) {
        int tagged = 0;
        int val = *(int *)((const char *)self + 0x10);
        int tag = val & 1;
        if (tag != 0) {
            tagged = 1;
        }
        if (tagged != 0) {
            val = 0;
        } else {
            __asm__ volatile("" ::: "memory");
        }

        int check = val;
        if (check != 0) {
            char *type = *(char **)(check + 4);
            GetTextSlot *slot = (GetTextSlot *)(type + 0xD0);
            ((void (*)(void *, char *))slot->fn)(
                (char *)val + slot->offset, out);
        } else {
            cStrCat(out, (const char *)0x36DB24);
        }
    } else {
        int owned = 0;
        int obj = *(int *)((const char *)self + 0x20);
        int objTag = obj & 1;
        if (objTag != 0) {
            owned = 1;
        }

        int useFallback;
        if (owned == 0) {
            goto first_object_not_owned;
        }
        useFallback = 1;
        goto first_object_fallback_done;
    first_object_not_owned:
        int zero = obj == 0;
        zero &= 0xFF;
        useFallback = zero != 0;
    first_object_fallback_done:

        if (useFallback != 0) {
            cStrAppend(out, (const char *)0x36DACC);
        } else {
            local0[0] = '\0';

            int ownedAgain = 0;
            if (objTag != 0) {
                ownedAgain = 1;
            }

            char *type;
            int base = obj;
            if (ownedAgain != 0) {
                base = 0;
                type = *(char **)(base + 4);
            } else {
                type = *(char **)(base + 4);
            }

            GetTextSlot *slot = (GetTextSlot *)(type + 0x40);
            ((void (*)(void *, char *))slot->fn)(
                (char *)base + slot->offset, local0);
            cStrAppend(out, local0);
        }
    }
    cStrAppend(out, close);

    cStrAppend(out, (const char *)0x36DCB8, (const char *)0x36DAF0);

    int kind = *(int *)((const char *)self + 8);
    int isFive = kind == 5;
    isFive &= 0xFF;
    if (isFive != 0) {
        cStrAppend(out, (const char *)0x36E300);

        int tagged = 0;
        int val = *(int *)((const char *)self + 0x14);
        int tag = val & 1;
        if (tag != 0) {
            tagged = 1;
        }
        if (tagged != 0) {
            val = 0;
        } else {
            __asm__ volatile("" ::: "memory");
        }

        int check = val;
        if (check != 0) {
            char *type = *(char **)(check + 4);
            GetTextSlot *slot = (GetTextSlot *)(type + 0xD0);
            ((void (*)(void *, char *))slot->fn)(
                (char *)val + slot->offset, out);
        } else {
            cStrCat(out, (const char *)0x36DB24);
        }
        cStrAppend(out, close);
        kind = *(int *)((const char *)self + 8);
    }

    int two = 2;
    int flag = 0;
    if ((kind == two) || (kind == 3) || (kind == 4)) {
        flag = 1;
    }
    flag &= 0xFF;

    int isFour = kind ^ 4;
    if (flag != 0) {
        cStrAppend(out, (const char *)0x36E300);

        int tagged = 0;
        int val = *(int *)((const char *)self + 0x18);
        int tag = val & 1;
        if (tag != 0) {
            tagged = 1;
        }
        if (tagged != 0) {
            val = 0;
        } else {
            __asm__ volatile("" ::: "memory");
        }

        int check = val;
        if (check != 0) {
            char *type = *(char **)(check + 4);
            GetTextSlot *slot = (GetTextSlot *)(type + 0xD0);
            ((void (*)(void *, char *))slot->fn)(
                (char *)val + slot->offset, out);
        } else {
            cStrCat(out, (const char *)0x36DB24);
        }
        cStrAppend(out, close);
        isFour = *(int *)((const char *)self + 8) ^ 4;
    }

    isFour = isFour == 0;
    isFour &= 0xFF;
    if (isFour != 0) {
        cStrAppend(out, (const char *)0x36E300);

        if (*(int *)((const char *)self + 0x0C) == 0) {
            int tagged = 0;
            int val = *(int *)((const char *)self + 0x1C);
            int tag = val & 1;
            if (tag != 0) {
                tagged = 1;
            }
            if (tagged != 0) {
                val = 0;
            } else {
                __asm__ volatile("" ::: "memory");
            }

            int check = val;
            if (check != 0) {
                char *type = *(char **)(check + 4);
                GetTextSlot *slot = (GetTextSlot *)(type + 0xD0);
                ((void (*)(void *, char *))slot->fn)(
                    (char *)val + slot->offset, out);
            } else {
                cStrCat(out, (const char *)0x36DB24);
            }
        } else {
            int obj = *(int *)((const char *)self + 0x24);
            int owned = 0;
            register int objTag __asm__("$16") = obj & 1;
            if (objTag != 0) {
                owned = 1;
            }

            int useFallback;
            if (owned == 0) {
                goto second_object_not_owned;
            }
            useFallback = 1;
            goto second_object_fallback_done;
        second_object_not_owned:
            int zero = obj == 0;
            zero &= 0xFF;
            useFallback = zero != 0;
        second_object_fallback_done:

            if (useFallback != 0) {
                cStrAppend(out, (const char *)0x36DACC);
            } else {
                local1[0] = '\0';
                register char *local1Start __asm__("$16") = local1;

                int ownedAgain = 0;
                if (objTag != 0) {
                    ownedAgain = 1;
                }

                char *type;
                int base = obj;
                if (ownedAgain != 0) {
                    base = 0;
                    type = *(char **)(base + 4);
                } else {
                    type = *(char **)(base + 4);
                }

                GetTextSlot *slot = (GetTextSlot *)(type + 0x40);
                ((void (*)(void *, char *))slot->fn)(
                    (char *)base + slot->offset, local1Start);
                cStrAppend(out, local1Start);
            }
        }
        cStrAppend(out, close);
    }
}
