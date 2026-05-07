// ODR-WARNING: this TU intentionally redeclares gcDoPlayCinematic with a
// minimal member list because include/gcDoPlayCinematic.h does not currently
// declare GetText. Keep new methods local to this file unless the shared
// header is intentionally expanded for all split TUs.
#include "cBase.h"

class cMemPool;
class cType;

struct GetTextSlot {
    short offset;
    short pad;
    void *fn;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cGUID {};
class cName {};

class cObject {
public:
    static int GetNameFromGUID(const cGUID &, const cType *, bool, bool,
                               cName *);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class gcStreamedCinematic {
public:
    static cBase *New(cMemPool *, cBase *);
};

class gcDoPlayCinematic {
public:
    void GetText(char *) const;
};

struct cHandleNode {
    char pad0[8];
    char name[0x14];
    short nameLen;
    short nameCap;
    char pad[0x10];
    int id;
};

void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);
void cStrCopy(char *, const char *);

extern cHandleNode *handle_table[] asm("D_00048890");

extern cType *type_base asm("D_000385DC");
extern cType *type_named asm("D_000385E0");
extern cType *type_factory asm("D_000385E4");
extern cType *type_folder asm("D_00040C90");
extern cType *type_streamed_cinematic asm("D_00099AD4");

static inline __attribute__((always_inline)) void
append_handle_text(char *out, const char *fmt, int handle, char *scratch) {
    cHandleNode *node = 0;
    if (handle != 0) {
        cHandleNode *entry = handle_table[handle & 0xFFFF];
        if ((entry != 0) && (entry->id == handle)) {
            node = entry;
        }
    }

    const char *text;
    if (node != 0) {
        text = (const char *)node + 8;
        int empty = node->nameLen < 1;
        empty &= 0xFF;
        if (empty != 0) {
            text = (const char *)0x36DAB8;
        }
    } else {
        if (handle != 0) {
            text = (const char *)0x36DAC4;
        } else {
            text = (const char *)0x36DACC;
        }
    }

    cStrAppend(out, fmt, text, scratch);
}

void gcDoPlayCinematic::GetText(char *buf) const {
    register const gcDoPlayCinematic *self __asm__("$16") = this;
    register char *out __asm__("$17") = buf;
    char scratch[256];
    char name[24];

    cStrAppend(out, (const char *)0x36EDAC, (const char *)0x36DAF0);

    int mode = *(int *)((const char *)self + 0x0C);
    scratch[0] = '\0';
    int doObjectText = mode != 2;
    doObjectText &= 0xFF;
    if (doObjectText != 0) {
        int value = *(int *)((const char *)self + 0x20);
        register int owned __asm__("$5") = 0;
        register int tagged __asm__("$6") = value & 1;
        if (tagged != 0) {
            owned = 1;
        }

        int valid;
        if (owned != 0) {
            valid = 0;
        } else {
            valid = value != 0;
            valid &= 0xFF;
            valid = valid != 0;
        }

        if (valid != 0) {
            int ownedAgain = 0;
            if (tagged != 0) {
                ownedAgain = 1;
            }

            int base = value;
            char *type;
            if (ownedAgain != 0) {
                base = 0;
                type = *(char **)(base + 4);
            } else {
                type = *(char **)(base + 4);
            }

            GetTextSlot *slot = (GetTextSlot *)(type + 0x78);
            ((void (*)(void *, char *))slot->fn)(
                (char *)base + slot->offset, scratch);
            mode = *(int *)((const char *)self + 0x0C);
        } else {
            cStrCopy(scratch, (const char *)0x36DACC);
            mode = *(int *)((const char *)self + 0x0C);
        }
    }

    switch (mode) {
    case 1: {
        register char *nameStart __asm__("$18") = name;
        register const cGUID *guid __asm__("$19") =
            (const cGUID *)((const char *)self + 0x18);
        ((short *)nameStart)[10] = 0;
        ((short *)nameStart)[11] = 0;
        nameStart[0] = '\0';
        if (!type_streamed_cinematic) {
            if (!type_folder) {
                if (!type_factory) {
                    if (!type_named) {
                        if (!type_base) {
                            type_base = cType::InitializeType(
                                (const char *)0x36D894,
                                (const char *)0x36D89C, 1, 0, 0, 0, 0, 0);
                        }
                        type_named = cType::InitializeType(
                            0, 0, 2, type_base, cNamed::New, 0, 0, 0);
                    }
                    type_factory = cType::InitializeType(
                        0, 0, 3, type_named, 0, 0, 0, 0);
                }
                type_folder = cType::InitializeType(
                    0, 0, 5, type_factory, 0, 0, 0, 0);
            }
            type_streamed_cinematic = cType::InitializeType(
                0, 0, 0x16B, type_folder, gcStreamedCinematic::New,
                (const char *)0x36D8F0, (const char *)0x36D8FC, 1);
        }

        cObject::GetNameFromGUID(
            *guid, type_streamed_cinematic, false, false, (cName *)nameStart);
        cStrAppend(out, (const char *)0x36E9A0, nameStart, scratch);
        break;
    }
    case 0:
    case 5:
        append_handle_text(out, (const char *)0x36E9A0,
                           *(int *)((const char *)self + 0x10), scratch);
        break;
    case 3:
    case 6:
        append_handle_text(out, (const char *)0x36E9A0,
                           *(int *)((const char *)self + 0x14), scratch);
        break;
    case 4:
    case 7:
    case 8: {
        int value = *(int *)((const char *)self + 0x2C);
        int owned = 0;
        int tagged = value & 1;
        if (tagged != 0) {
            owned = 1;
        }
        if (owned != 0) {
            value = 0;
        } else {
            __asm__ volatile("" ::: "memory");
        }

        int check = value;
        if (check != 0) {
            char *type = *(char **)(check + 4);
            GetTextSlot *slot = (GetTextSlot *)(type + 0xD0);
            ((void (*)(void *, char *))slot->fn)(
                (char *)value + slot->offset, out);
        } else {
            cStrCat(out, (const char *)0x36DB24);
        }

        cStrAppend(out, (const char *)0x36DAD8);

        value = *(int *)((const char *)self + 0x30);
        owned = 0;
        tagged = value & 1;
        if (tagged != 0) {
            owned = 1;
        }
        if (owned != 0) {
            value = 0;
        } else {
            __asm__ volatile("" ::: "memory");
        }

        check = value;
        if (check != 0) {
            char *type = *(char **)(check + 4);
            GetTextSlot *slot = (GetTextSlot *)(type + 0xD0);
            ((void (*)(void *, char *))slot->fn)(
                (char *)value + slot->offset, out);
        } else {
            cStrCat(out, (const char *)0x36DB24);
        }

        cStrAppend(out, (const char *)0x36DBAC, scratch);
        break;
    }
    case 2:
    default:
        break;
    }

    const char *suffix;
    if (**(int **)((const char *)self + 0x24) != 0) {
        suffix = (const char *)0x36DAD8;
    } else {
        suffix = (const char *)0x36E440;
    }
    cStrCat(out, suffix);
}
