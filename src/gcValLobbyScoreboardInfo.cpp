// gcValLobbyScoreboardInfo -- decompiled from gcAll_psp.obj

class cBase;
class cFile;
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

class cWriteBlock {
public:
    int _data[2];

    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void WriteBase(const cBase *);
    void End(void);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcValue {
public:
    cBase *mParent;
    void *mVtable;

    void Write(cFile &) const;
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
};

void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);

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
