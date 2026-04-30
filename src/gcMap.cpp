#include "gcMap.h"
#include "gcStreamedCinematic.h"

inline void *operator new(unsigned int, void *p) { return p; }

class cFile;
class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    cFile *_file;
    int _pos;

    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned int);
    void Write(int, const float *);
    void End(void);
};

class cBaseArray {
public:
    void Write(cWriteBlock &) const;
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

class eWorld;

class eWeatherSystem {
public:
    void AddToWorld(eWorld *);
};

class gcRegionBase {
public:
    int AddToWorld(void);
    void Write(cFile &) const;
};

class gcViewport {
public:
    static void AddToWorld(eWorld *);
};

class gcCinematicInstance {
public:
    char pad_00[0x38];
    unsigned char mStopFlag;  // 0x38

    void Stop(bool);
};

class gcRegionSetGroup {
public:
    void ClearRegionSetState(int) const;
};

struct TypeMethodPool {
    short offset;
    short _pad;
    void (*fn)(void *, cMemPool *, int);
};

struct TypeMethodInt2 {
    short offset;
    short _pad;
    void (*fn)(void *, int, int);
};

struct TypeMethod4 {
    short offset;
    short _pad;
    void (*fn)(void *, int);
};

struct TypeMethod1 {
    short offset;
    short _pad;
    void (*fn)(void *);
};

struct TypeMethod {
    short offset;
    short _pad;
    void (*fn)(void *, cFile *);
};

class TypedObject {
public:
    int _parent;
    cType *mType;
};

struct gcMapCinematicState {
    int mA;
    int mB;
};

struct gcMapRegionState {
    char pad_00[0x64];
    void *mWorld;            // 0x64
    char pad_68[0x18];
    gcMapRegionState *mPrev; // 0x80
    gcMapRegionState *mNext; // 0x84
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00040C90;
extern cType *D_00099AE0;
extern cType *D_00099B04;
extern gcMapRegionState *D_0037D0EC;
extern int gcStreamedCinematic_currentIndex;
extern gcStreamedCinematic *gcStreamedCinematic_table[];

struct gcMap_AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

cBase *gcMap::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    gcMap_AllocRec *rec = (gcMap_AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcMap *result = 0;
    gcMap *obj = (gcMap *)rec->fn(base, 0x430, 0x10, 0, 0);
    if (obj != 0) {
        new (obj) gcMap(parent, 0);
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcMap::GetType(void) const {
    if (D_00099B04 == 0) {
        if (D_00099AE0 == 0) {
            if (D_00040C90 == 0) {
                if (D_000385E4 == 0) {
                    if (D_000385E0 == 0) {
                        if (D_000385DC == 0) {
                            D_000385DC = cType::InitializeType(
                                (const char *)0x36D894, (const char *)0x36D89C,
                                1, 0, 0, 0, 0, 0);
                        }
                        D_000385E0 = cType::InitializeType(
                            0, 0, 2, D_000385DC,
                            (cBase *(*)(cMemPool *, cBase *))0x1C3C58,
                            0, 0, 0);
                    }
                    D_000385E4 = cType::InitializeType(
                        0, 0, 3, D_000385E0, 0, 0, 0, 0);
                }
                D_00040C90 = cType::InitializeType(
                    0, 0, 5, D_000385E4, 0, 0, 0, 0);
            }
            D_00099AE0 = cType::InitializeType(
                0, 0, 0xB3, D_00040C90, 0, 0, 0, 0);
        }
        D_00099B04 = cType::InitializeType(
            0, 0, 0x65, D_00099AE0, &gcMap::New,
            (const char *)0x36D938, (const char *)0x36D940, 1);
    }
    return D_00099B04;
}

void gcMap::Write(cFile &file) const {
    cWriteBlock wb(file, 0x10);
    ((const gcRegionBase *)this)->Write(file);
    wb.Write(*(const unsigned int *)((const char *)this + 0x20C));
    ((const cBaseArray *)((const char *)this + 0x398))->Write(wb);

    {
        char *typePtr = *(char **)((const char *)this + 0x7C);
        TypeMethod *entry = (TypeMethod *)(typePtr + 0x28);
        char *base = (char *)this + 0x78;
        entry->fn(base + entry->offset, wb._file);
    }

    {
        char *typePtr = *(char **)((const char *)this + 0x98);
        TypeMethod *entry = (TypeMethod *)(typePtr + 0x28);
        char *base = (char *)this + 0x94;
        entry->fn(base + entry->offset, wb._file);
    }

    {
        char *typePtr = *(char **)((const char *)this + 0xB4);
        TypeMethod *entry = (TypeMethod *)(typePtr + 0x28);
        char *base = (char *)this + 0xB0;
        entry->fn(base + entry->offset, wb._file);
    }

    {
        char *typePtr = *(char **)((const char *)this + 0xD0);
        TypeMethod *entry = (TypeMethod *)(typePtr + 0x28);
        char *base = (char *)this + 0xCC;
        entry->fn(base + entry->offset, wb._file);
    }

    {
        char *typePtr = *(char **)((const char *)this + 0xEC);
        TypeMethod *entry = (TypeMethod *)(typePtr + 0x28);
        char *base = (char *)this + 0xE8;
        entry->fn(base + entry->offset, wb._file);
    }

    {
        char *typePtr = *(char **)((const char *)this + 0x124);
        TypeMethod *entry = (TypeMethod *)(typePtr + 0x28);
        char *base = (char *)this + 0x120;
        entry->fn(base + entry->offset, wb._file);
    }

    {
        char *typePtr = *(char **)((const char *)this + 0x108);
        TypeMethod *entry = (TypeMethod *)(typePtr + 0x28);
        char *base = (char *)this + 0x104;
        entry->fn(base + entry->offset, wb._file);
    }

    {
        TypeMethod *entry =
            (TypeMethod *)((char *)*(void **)((const char *)this + 0x3F0) + 0x28);
        void *base = (char *)this + 0x3EC;
        entry->fn((char *)base + entry->offset, &file);
    }

    {
        TypedObject *obj = *(TypedObject *const *)((const char *)this + 0x1F4);
        TypeMethod *entry = (TypeMethod *)((char *)obj->mType + 0x28);
        entry->fn((char *)obj + entry->offset, &file);
    }

    ((const cHandle *)((const char *)this + 0x1F8))->Write(wb);
    wb.Write(3, (const float *)((const char *)this + 0x420));
    ((const cHandle *)((const char *)this + 0x1F0))->Write(wb);
    wb.End();
}

void gcMap::PostUpdate(void) {
    HandleCinematicSkip();
}

int gcMap::AddToWorld(void) {
    gcViewport::AddToWorld(*(eWorld **)((char *)this + 0x50));

    gcMapCinematicState state;
    gcMapCinematicState *statePtr = &state;
    int *worldField = (int *)((char *)*(int *)((char *)this + 0x50) + 0x20);
    int *valuePtr = (int *)((char *)this + 0x58);
    int value = *valuePtr;
    statePtr->mA = value;
    statePtr->mB = value;
    *worldField = value;

    if (!((gcRegionBase *)this)->AddToWorld()) {
        return 0;
    }

    ((eWeatherSystem *)*(int *)((char *)this + 0x1F4))
        ->AddToWorld(*(eWorld **)((char *)this + 0x50));

    gcMapRegionState *region = *(gcMapRegionState **)((char *)this + 0x1FC);
    void *world = *(void **)((char *)this + 0x50);
    if (world == region->mWorld) {
    } else {
        TypeMethodInt2 *entry70 =
            (TypeMethodInt2 *)(*(char **)((char *)region + 4) + 0x70);
        region->mWorld = world;
        entry70->fn((char *)region + entry70->offset, 4, 0);

        if (region->mWorld != 0) {
            if (region->mPrev == 0 || region->mNext == 0) {
                gcMapRegionState *head = D_0037D0EC;
                if (head != 0) {
                    region->mPrev = head->mPrev;
                    region->mNext = D_0037D0EC;
                    region->mPrev->mNext = region;
                    region->mNext->mPrev = region;
                } else {
                    D_0037D0EC = region;
                    region->mNext = region;
                    region->mPrev = region;
                }
            }
        }

        TypeMethod1 *entryC0 =
            (TypeMethod1 *)(*(char **)((char *)region + 4) + 0xC0);
        entryC0->fn((char *)region + entryC0->offset);
    }

    int i = 0;
    gcMap *regions = this;
    do {
        gcRegion *loaded = regions->mLoadedRegions[0];
        if (loaded != 0) {
            char *typePtr = *(char **)((char *)loaded + 4);
            TypeMethod1 *entry = (TypeMethod1 *)(typePtr + 0xC0);
            entry->fn((char *)loaded + entry->offset);
        }
        i++;
        regions = (gcMap *)((char *)regions + 4);
    } while (i < 2);

    return 1;
}

void gcMap::LoadObjectBackgroundBegin(cHandleT<gcEntity> handle) {
    if (!(mFlags & 0x40000)) {
        mLoadBackgroundHandle = handle;
        *(int *)0x0037D7F8 = 0;
        mFlags |= 0x40000;
    }
}

void gcMap::UnloadAllRegions(void) {
    for (int i = 0; i < 2; i++) {
        DeleteRegion(i);
    }
}

void gcMap::CancelCurrentCinematic(void) {
    gcStreamedCinematic *other =
        gcStreamedCinematic_table[gcStreamedCinematic_currentIndex < 1];

    if (!(mFlags & 8) && other != 0 && *(unsigned char *)((char *)other + 0x99) != 0) {
        char *typePtr = *(char **)((char *)this + 4);
        TypeMethodPool *entry58 = (TypeMethodPool *)(typePtr + 0x58);
        entry58->fn((char *)this + entry58->offset,
                    cMemPool::GetPoolFromPtr(other), 0);

        typePtr = *(char **)((char *)other + 4);
        TypeMethod4 *entry50 = (TypeMethod4 *)(typePtr + 0x50);
        entry50->fn((char *)other + entry50->offset, 3);

        gcMapCinematicState cleared;
        cleared.mA = 0;
        cleared.mB = 0;
        gcMapCinematicState *state =
            (gcMapCinematicState *)((char *)this + 0x3DC);
        *state = cleared;
        mFlags &= 0xFFDFFFFF;
        *(int *)((char *)this + 0x3D8) = 0;
    }

    gcStreamedCinematic *current =
        gcStreamedCinematic_table[gcStreamedCinematic_currentIndex];
    if (current != 0 && *(unsigned char *)((char *)current + 0x99) != 0) {
        gcCinematicInstance *instance = current->mpCinematicInstance;
        if (instance != 0 && instance->mStopFlag == 0) {
            gcMapCinematicState *state =
                (gcMapCinematicState *)((char *)this + 0x3DC);
            gcMapCinematicState saved;
            gcMapCinematicState *savedPtr = &saved;
            *savedPtr = *state;
            instance->Stop(false);

            int same = 0;
            if (state->mA == savedPtr->mA) {
                same = (unsigned char)same;
                if (state->mB == savedPtr->mB) {
                    same = 1;
                }
            } else {
                same = (unsigned char)same;
            }

            if (same != 0) {
                gcMapCinematicState cleared;
                cleared.mA = 0;
                cleared.mB = 0;
                *state = cleared;
                mFlags &= 0xFFDFFFFF;
            }
        }
    }
}

void gcMap::DeleteDynamicLoadedObjects(int arg0, cHandleT<gcEntity> handle) {
    if (!(mFlags & 0x100000) && !(mFlags & 0x80000)) {
        mFlags |= 0x80000;
        mDeleteDynamicHandle = handle;
        *(int *)0x0037D7F4 = arg0;
    }
}

void gcMap::ResetRegionStates(void) {
    int count;
    if (mObjectLoadArray != 0) {
        count = mObjectLoadArray[-1] & 0x3FFFFFFF;
    } else {
        count = 0;
    }
    for (int i = 0; i < count; i++) {
        mObjectLoadArray[i] = -1;
    }
    mLoadedCount = 0;
}

void gcMap::CaptureRegionStates(void) {
    int i = 0;
    gcMap *regions = this;
    gcMap *map = this;
    do {
        gcRegionBase *region = (gcRegionBase *)regions->mLoadedRegions[0];
        if (region != 0) {
            map->CaptureRegionState(region);
        }
        i++;
        regions = (gcMap *)((char *)regions + 4);
    } while (i < 2);
}

void gcMap::ApplyRegionStates(void) {
    int i = 0;
    gcMap *regions = this;
    gcMap *map = this;
    do {
        gcRegionBase *region = (gcRegionBase *)regions->mLoadedRegions[0];
        if (region != 0) {
            map->ApplyRegionState(region);
        }
        i++;
        regions = (gcMap *)((char *)regions + 4);
    } while (i < 2);
}

gcRegion *gcMap::FindLoadedRegion(const cGUIDT<gcRegion> &guid) const {
    int i = 0;
    gcRegion *const *regions = mLoadedRegions;
    do {
        gcRegion *region = *regions;
        if (region != 0) {
            int match = 0;
            if (*(int *)((char *)region + 0x20) == guid.mA) {
                match = (unsigned char)match;
                if (*(int *)((char *)region + 0x24) == guid.mB) {
                    match = 1;
                    goto matched_path;
                }
            } else {
matched_path:
                match = (unsigned char)match;
            }
            if (match != 0) {
                return region;
            }
        }
        i++;
        regions++;
    } while (i < 2);
    return 0;
}

void gcMap::ClearRegionSetState(int index, int state) {
    gcRegionSetGroup *group;
    gcRegionSetGroup **groups;
    if (index >= 0) {
        groups = *(gcRegionSetGroup ***)((char *)this + 0x398);
        int count = 0;
        if (groups != 0) {
            count = ((int *)groups)[-1];
        }
        if (index < count) {
            goto valid;
        }
    }
    group = 0;
    goto done;
valid:
    group = *(gcRegionSetGroup **)((char *)groups + (index << 2));
done:
    if (group != 0) {
        group->ClearRegionSetState(state);
    }
}
