#include "eWorld.h"
#include "mVec3.h"

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

extern "C" void *cMemPool_GetPoolFromPtr(const void *);

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

extern "C" void eWorld___dtor_eWorld_void(eWorld *self, int flags) {
    if (self != 0) {
        int i = 0;
        do {
            i++;
        } while (i < 3);
        if (flags & 1) {
            if (self != 0) {
                void *pool = cMemPool_GetPoolFromPtr(self);
                void *block = *(void **)((char *)pool + 0x24);
                DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
                short off = rec->offset;
                rec->fn((char *)block + off, self);
            }
        }
    }
}
