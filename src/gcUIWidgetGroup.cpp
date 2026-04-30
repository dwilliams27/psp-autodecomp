#include "gcUIWidgetGroup.h"
#include "gcUIWidget.h"

inline void *operator new(unsigned int, void *p) { return p; }

class cType {
public:
    char _pad[0x1C];
    const cType *mParent;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class eSprite;

template <class T> class cHandleT {
public:
    int _data;
};

template <class T> class cArrayBase {
public:
    T *mData;
    cArrayBase &operator=(const cArrayBase &);
};

struct cTypeMethod {
    short offset;
    short pad;
    const cType *(*fn)(const void *);
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_0009990C;
extern cType *D_00099920;

extern "C" void gcUIWidgetGroup_GetCellPos_vec(const gcUIWidgetGroup *, float *,
                                               const short *)
    __asm__("__0fPgcUIWidgetGroupKGetCellPos6IgcUICellK");

gcUIWidgetGroup::gcUIWidgetGroup(cBase *parent) : gcUIWidget(parent) {
    *(void **)((char *)this + 4) = (void *)0x387058;
    *(int *)((char *)this + 0xB0) = 0x21;
    *(float *)((char *)this + 0xB4) = 0.0f;
    *(float *)((char *)this + 0xB8) = 0.0f;
    *(int *)((char *)this + 0xBC) = 0;
    *(int *)((char *)this + 0xC0) = 0;
    *(int *)((char *)this + 0xC4) = 0;
    *(int *)((char *)this + 0xC8) = 0;
    *(float *)((char *)this + 0xCC) = 0.0f;
    *(float *)((char *)this + 0xD0) = 0.0f;
    *(short *)((char *)this + 0xD4) = 0;
}

gcUIWidgetGroup &gcUIWidgetGroup::operator=(const gcUIWidgetGroup &other) {
    ((gcUIWidget *)this)->operator=(*(const gcUIWidget *)&other);
    DeleteAllWidgets(true);

    int width = 0;
    const int *srcWidthArray = *(const int *const *)((const char *)&other + 0xC0);
    const int *srcHeightArray =
        *(const int *const *)((const char *)&other + 0xC4);
    if (srcWidthArray != 0) {
        width = srcWidthArray[-1] & 0x3FFFFFFF;
    }

    int height = 0;
    if (srcHeightArray != 0) {
        height = srcHeightArray[-1] & 0x3FFFFFFF;
    }

    SetDimensions(width, height);
    *(int *)((char *)this + 0xB0) =
        *(const int *)((const char *)&other + 0xB0);
    ((cArrayBase<cHandleT<eSprite> > *)((char *)this + 0xC8))->operator=(
        *(const cArrayBase<cHandleT<eSprite> > *)((const char *)&other + 0xC8));
    *(float *)((char *)this + 0xB4) =
        *(const float *)((const char *)&other + 0xB4);
    *(float *)((char *)this + 0xB8) =
        *(const float *)((const char *)&other + 0xB8);
    *(float *)((char *)this + 0xCC) =
        *(const float *)((const char *)&other + 0xCC);
    *(float *)((char *)this + 0xD0) =
        *(const float *)((const char *)&other + 0xD0);

    cMemPool *pool = cMemPool::GetPoolFromPtr(this);

    int dstWidth = 0;
    const int *dstWidthArray = *(const int *const *)((char *)this + 0xC0);
    const int *dstHeightArray = *(const int *const *)((char *)this + 0xC4);
    if (dstWidthArray != 0) {
        dstWidth = dstWidthArray[-1] & 0x3FFFFFFF;
    }

    int dstHeight = 0;
    if (dstHeightArray != 0) {
        dstHeight = dstHeightArray[-1] & 0x3FFFFFFF;
    }

    int cellCount = dstWidth;
    cellCount *= dstHeight;
    int cell = 0;
    if (cell < cellCount) {
        __asm__ volatile("" ::: "memory");
        char *cellHeads = *(char **)((const char *)&other + 0xBC);
        int offset = 0;
        do {
            gcUIWidget *widget = *(gcUIWidget **)(cellHeads + offset);
            if (widget != 0) {
                do {
                    char *dispatch = *(char **)((char *)widget + 4);
                    struct CopyEntry {
                        short offset;
                        short pad;
                        gcUIWidget *(*fn)(void *, cMemPool *, cBase *);
                    };
                    CopyEntry *entry = (CopyEntry *)(dispatch + 0x10);
                    gcUIWidget *copy =
                        entry->fn((char *)widget + entry->offset, pool,
                                  (cBase *)this);
                    InsertInDrawOrder(cell, copy, 0);
                    widget = *(gcUIWidget **)((char *)widget + 0x68);
                    cellHeads = *(char **)((const char *)&other + 0xBC);
                } while (widget != *(gcUIWidget **)(cellHeads + offset));
            }
            cell += 1;
            offset += 4;
            cellHeads = *(char **)((const char *)&other + 0xBC);
        } while (cell < cellCount);
    }

    return *this;
}

cBase *gcUIWidgetGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcUIWidgetGroup *result = 0;
    gcUIWidgetGroup *obj = (gcUIWidgetGroup *)rec->fn(base, 0xD8, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcUIWidgetGroup(parent);
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcUIWidgetGroup::GetType(void) const {
    if (D_00099920 == 0) {
        if (D_0009990C == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(
                    0, 0, 2, D_000385DC,
                    (cBase *(*)(cMemPool *, cBase *))0x1C3C58, 0, 0, 0);
            }
            D_0009990C =
                cType::InitializeType(0, 0, 0x84, D_000385E0, 0, 0, 0, 0);
        }
        D_00099920 = cType::InitializeType(0, 0, 0x85, D_0009990C,
                                           gcUIWidgetGroup::New, 0, 0, 0);
    }
    return D_00099920;
}

void gcUIWidgetGroup::AssignCopy(const cBase *base) {
    gcUIWidgetGroup *other = 0;

    if (base != 0) {
        if (D_00099920 == 0) {
            if (D_0009990C == 0) {
                if (D_000385E0 == 0) {
                    if (D_000385DC == 0) {
                        D_000385DC =
                            cType::InitializeType((const char *)0x36D894,
                                                  (const char *)0x36D89C,
                                                  1, 0, 0, 0, 0, 0);
                    }
                    D_000385E0 = cType::InitializeType(
                        0, 0, 2, D_000385DC,
                        (cBase *(*)(cMemPool *, cBase *))0x1C3C58, 0, 0, 0);
                }
                D_0009990C =
                    cType::InitializeType(0, 0, 0x84, D_000385E0, 0, 0, 0, 0);
            }
            D_00099920 = cType::InitializeType(0, 0, 0x85, D_0009990C,
                                               gcUIWidgetGroup::New, 0, 0, 0);
        }

        const cType *type = D_00099920;
        const cTypeMethod *method =
            (const cTypeMethod *)(*(char **)((const char *)base + 4) + 8);
        const cType *current = method->fn((const char *)base + method->offset);
        int match;

        if (type == 0) {
            match = 0;
        } else if (current != 0) {
loop:
            if (current == type) {
                match = 1;
            } else {
                current = current->mParent;
                if (current != 0) {
                    goto loop;
                }
                match = 0;
            }
        } else {
            match = 0;
        }
        if (match != 0) {
            other = (gcUIWidgetGroup *)base;
        }
    }
    this->operator=(*other);
}

int gcUIWidgetGroup::NeedsEffectUpdate(void) const {
    int result = 0;
    int groupEffect = ((*(unsigned int *)((char *)this + 0xB0) & 0x20000000) != 0) & 0xFF;
    if (groupEffect ||
        (((*(unsigned int *)((char *)this + 0x24) & 0x08000000) != 0) & 0xFF)) {
        result = 1;
    }
    return result & 0xFF;
}

void *gcUIWidgetGroup::GetResizableSprites(void) {
    int localSprites = ((*(unsigned int *)((char *)this + 0xB0) & 8) != 0) & 0xFF;
    if (localSprites) {
        return (char *)this + 0xC8;
    }
    return ((gcUIWidget *)this)->GetResizableSprites();
}

float gcUIWidgetGroup::GetCellPos(gcUICell cell, int axis) const {
    float work[2];
    float *pos = work;
    gcUICell local = cell;
    gcUIWidgetGroup_GetCellPos_vec(this, pos, (const short *)&local);
    return pos[axis];
}
