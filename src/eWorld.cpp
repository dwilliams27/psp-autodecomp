#include "eWorld.h"
#include "mVec3.h"

class eCamera;
class eCameraBins;
class eMaterial;
class ePortal;
template <class T> class cHandleT {
public:
    int handle;
    cHandleT() : handle(0) {}
};

typedef int eWorld_v4sf_t __attribute__((mode(V4SF)));

struct mSphere {
    eWorld_v4sf_t v;
};

class mFrustum {
public:
    int Clip(const mSphere &, unsigned int) const;
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class eGeom {
public:
    static void CleanNeedsUpdateList(void);
};

class eCameraEffectMgr {
public:
    void Cull(unsigned int, const eCamera &, eCameraBins *) const;
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class eRoomAABBTree {
public:
    int Contains(const mVec3 &) const;
    void Cull(unsigned int, const eCamera &, const mFrustum &, eCameraBins *,
              int, const cHandleT<eMaterial> *, float) const;
};

struct HandleEntry {
    char pad[48];
    int handle;
};

struct HandleValue {
    int value;
};

struct HandleRawValues {
    int pad;
    int room;
    int set;
};

extern HandleEntry *D_00038890[];

static int sNextCullId;

struct eWorldCullCallRec {
    short offset;
    short pad;
    void (*fn)(void *, unsigned int, const eCamera &, const mFrustum &,
               eCameraBins *, int, int, const cHandleT<eMaterial> *, float);
};

struct eWorldUpdateRec {
    short offset;
    short pad;
    void (*fn)(void *);
};

struct eWorldCullGeom {
    char pad0[4];
    char *typeInfo;
    char pad8[0x38];
    mSphere sphere;
    char pad50[0x20];
    eWorldCullGeom *next;
    float radiusA;
    float radiusB;
    char pad7C[0x10];
    unsigned char flags;
    unsigned char mask;
};

void eWorld::LockWorld(bool lock) const {
    if (lock) {
        lockCount++;
    } else {
        lockCount--;
    }
}

int eWorld::GetNextCullId(void) {
    return sNextCullId++;
}

eWorld::eWorld() {
    field_0x00 = 0;
    roomList = 0;
    roomSetList = 0;
    soundList = 0;
    field_0x1C = 0;
    field_0x20 = 0;
    lockCount = 0;
    int i = 0;
    int *p = (int *)this;
    do {
        p[1] = 0;
        i++;
        p++;
    } while (i < 3);
}

void eWorld::AddSound(eSound *s) {
    eSound *head = soundList;
    if (head) {
        s->nextSound = head->nextSound;
        s->prevSound = soundList;
        eSound *n = s->nextSound;
        n->prevSound = s;
        eSound *p = s->prevSound;
        p->nextSound = s;
    } else {
        soundList = s;
        s->prevSound = s;
        s->nextSound = s;
    }
}

void eWorld::AddRoom(eRoom *r) {
    eRoom *head = roomList;
    if (head) {
        r->nextRoom = head->nextRoom;
        r->prevRoom = roomList;
        eRoom *n = r->nextRoom;
        n->prevRoom = r;
        eRoom *p = r->prevRoom;
        p->nextRoom = r;
    } else {
        roomList = r;
        r->prevRoom = r;
        r->nextRoom = r;
    }
}

void eWorld::AddRoomSet(eRoomSet *rs) {
    eRoomSet *head = roomSetList;
    if (head) {
        rs->nextRoomSet = head->nextRoomSet;
        rs->prevRoomSet = roomSetList;
        eRoomSet *n = rs->nextRoomSet;
        n->prevRoomSet = rs;
        eRoomSet *p = rs->prevRoomSet;
        p->nextRoomSet = rs;
    } else {
        roomSetList = rs;
        rs->prevRoomSet = rs;
        rs->nextRoomSet = rs;
    }
}

int eWorld::IsPointInFluidVolume(const eRoom *room, const mVec3 &pos) const {
    const eRoom *r = GetRoomFromPos(room, pos);
    if (r) {
        return r->IsPointInFluidVolume(pos);
    }
    return 0;
}

const eRoom *eWorld::GetRoomFromPos(const eRoom *room, const mVec3 &pos) const {
    volatile HandleValue roomHeld;
    volatile HandleValue setHeld;
    volatile HandleRawValues raw;

    if (room != 0) {
        raw.room = room->Contains(pos);
        roomHeld.value = raw.room;
        int value = roomHeld.value;
        HandleEntry *found;
        if (value == 0) {
            found = 0;
        } else {
            HandleEntry *entry = D_00038890[value & 0xFFFF];
            HandleEntry *valid = 0;
            if (entry != 0) {
                found = valid;
                if (entry->handle == value) {
                    valid = entry;
                }
            }
            found = valid;
        }
        if (found != 0) {
            return (const eRoom *)found;
        }
    }

    eRoomSet *set = roomSetList;
    if (set != 0) {
        do {
            raw.set = ((const eRoomAABBTree *)((char *)set + 0x44))->Contains(pos);
            setHeld.value = raw.set;
            int value = setHeld.value;
            HandleEntry *found;
            if (value == 0) {
                found = 0;
            } else {
                HandleEntry *entry = D_00038890[value & 0xFFFF];
                HandleEntry *valid = 0;
                if (entry != 0) {
                    found = valid;
                    if (entry->handle == value) {
                        valid = entry;
                    }
                }
                found = valid;
            }
            if (found != 0) {
                return (const eRoom *)found;
            }
            set = set->prevRoomSet;
        } while (set != roomSetList);
    }
    return 0;
}

#pragma control sched=1
void eWorld::Cull(const eCamera &camera, const mFrustum &frustum,
                  const mVec3 &pos, eCameraBins *bins, int pass,
                  const cHandleT<eMaterial> *material, float alpha) const {
    eGeom::CleanNeedsUpdateList();
    const eWorld *self = this;
    self->LockWorld(true);
    unsigned int stamp = GetNextCullId();

    if (pass == 0 && ((*(unsigned short *)((char *)bins + 4) & 0x10) == 0)) {
        int clips[3];
        unsigned char enabled[3];
        clips[0] = 0;
        clips[1] = 6;
        clips[2] = 2;
        enabled[0] = 1;
        enabled[1] = 1;
        enabled[2] = 0;

        int i = 0;
        do {
            *(unsigned char *)((char *)bins + 0x1770C) = enabled[i];
            *(int *)((char *)bins + 0x17710) = clips[i];
            eWorldCullGeom *head =
                *(eWorldCullGeom *const *)((const char *)self + 4 + i * 4);
            eWorldCullGeom *geom = head;
            if (head != 0) {
                do {
                    if (((geom->mask & *(unsigned short *)((char *)bins + 4)) != 0) ||
                        ((*(unsigned int *)0x37D0F0 & 0x2000) != 0)) {
                        if ((geom->flags & 0x10) != 0) {
                            char localFrustum[0x60];
                            const char *src = (const char *)&frustum;
                            char *dst = localFrustum;
                            unsigned int j = 0;
                            do {
                                float w = *(const float *)(src + 0x0C);
                                __asm__ volatile(
                                    "lv.q C120, 0(%1)\n"
                                    "sv.q C120, 0(%0)"
                                    :
                                    : "r"(dst), "r"(src)
                                    : "memory");
                                *(float *)(dst + 0x0C) = w;
                                j++;
                                dst += 0x10;
                                src += 0x10;
                            } while (j < 6);
                            *(float *)(localFrustum + 0x1C) =
                                -(*(float *)(localFrustum + 0x0C) +
                                  *(const float *)((const char *)&camera + 0x1E0));

                            eWorldCullCallRec *rec =
                                (eWorldCullCallRec *)(geom->typeInfo + 0x98);
                            rec->fn((char *)geom + rec->offset, stamp, camera,
                                    *(const mFrustum *)localFrustum, bins, 0x40,
                                    pass, material, alpha);
                        } else {
                            eWorldCullCallRec *rec =
                                (eWorldCullCallRec *)(geom->typeInfo + 0x98);
                            rec->fn((char *)geom + rec->offset, stamp, camera,
                                    frustum, bins, 0x40, pass, material, alpha);
                        }
                    }
                    geom = geom->next;
                } while (geom != head);
            }
            i++;
        } while (i < 3);
    }

    eRoom *room = *(eRoom *const *)((const char *)&camera + 0x258);
    if (room != 0) {
        int contained = room->Contains(pos);
        volatile int held = contained;
        int value = held;
        int invalid;
        if (value == 0) {
            invalid = 1;
        } else {
            HandleEntry *entry = D_00038890[value & 0xFFFF];
            HandleEntry *valid = 0;
            if (entry != 0) {
                if (entry->handle == value) {
                    valid = entry;
                }
            }
            invalid = valid == 0;
        }
        if ((invalid & 0xFF) != 0) {
            room = (eRoom *)GetRoomFromPos(0, pos);
        }
        if (room != 0) {
            cHandleT<ePortal> portal;
            cHandleT<eRoom> roomHandle;
            room->CullPortals(stamp, camera, bins, frustum, pos, portal,
                              roomHandle, pass, material, alpha);
        } else {
            goto cull_room_sets;
        }
    } else {
cull_room_sets:
        eRoomSet *set = self->roomSetList;
        if (set != 0) {
            do {
                ((const eRoomAABBTree *)((const char *)set + 0x44))
                    ->Cull(stamp, camera, frustum, bins, pass, material, alpha);
                set = set->prevRoomSet;
            } while (set != self->roomSetList);
        }
    }

    {
        eWorldCullGeom *head = *(eWorldCullGeom *const *)((const char *)self + 0);
        eWorldCullGeom *geom = head;
        if (head != 0) {
            do {
                if (((geom->mask & *(unsigned short *)((char *)bins + 4)) != 0) ||
                    ((*(unsigned int *)0x37D0F0 & 0x2000) != 0)) {
                    if ((geom->flags & 4) != 0) {
                        eWorldUpdateRec *rec =
                            (eWorldUpdateRec *)(geom->typeInfo + 0xB8);
                        rec->fn((char *)geom + rec->offset);
                    }

                    float radius = geom->radiusA * geom->radiusB;
                    mSphere sphere;
                    sphere.v = geom->sphere.v;
                    *(float *)((char *)&sphere + 0x0C) = radius;
                    int clip = frustum.Clip(sphere, 0x40);
                    if (clip != 0) {
                        eWorldCullCallRec *rec =
                            (eWorldCullCallRec *)(geom->typeInfo + 0x98);
                        rec->fn((char *)geom + rec->offset, stamp, camera,
                                frustum, bins, clip, pass, material, alpha);
                    }
                }
                geom = geom->next;
            } while (geom != head);
        }
    }

    if (pass == 0 && ((*(unsigned short *)((char *)bins + 4) & 0x80) != 0)) {
        ((const eCameraEffectMgr *)((const char *)&camera + 0x25C))
            ->Cull(stamp, camera, bins);
    }
    self->LockWorld(false);
}

#pragma control sched=2

void eWorld::RemoveRoom(eRoom *r) {
    if (r && r->nextRoom && r->prevRoom) {
        if (roomList == r) roomList = r->prevRoom;
        r->nextRoom->prevRoom = r->prevRoom;
        r->prevRoom->nextRoom = r->nextRoom;
        r->nextRoom = 0;
        r->prevRoom = 0;
        if (roomList == r) roomList = 0;
    }
}

void eWorld::RemoveRoomSet(eRoomSet *rs) {
    if (rs && rs->nextRoomSet && rs->prevRoomSet) {
        if (roomSetList == rs) roomSetList = rs->prevRoomSet;
        rs->nextRoomSet->prevRoomSet = rs->prevRoomSet;
        rs->prevRoomSet->nextRoomSet = rs->nextRoomSet;
        rs->nextRoomSet = 0;
        rs->prevRoomSet = 0;
        if (roomSetList == rs) roomSetList = 0;
    }
}

void eWorld::RemoveSound(eSound *s) {
    s->Stop();
    if (s && s->nextSound && s->prevSound) {
        if (soundList == s) soundList = s->prevSound;
        s->nextSound->prevSound = s->prevSound;
        s->prevSound->nextSound = s->nextSound;
        s->nextSound = 0;
        s->prevSound = 0;
        if (soundList == s) soundList = 0;
    }
}

inline void eWorld::operator delete(void *p) {
    if (p != 0) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
}

eWorld::~eWorld() {
    int i = 0;
    do {
        i++;
    } while (i < 3);
}
