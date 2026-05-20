#include "eWorld.h"
#include "mVec3.h"

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class eRoomAABBTree {
public:
    int Contains(const mVec3 &) const;
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
