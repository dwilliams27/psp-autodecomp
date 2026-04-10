#include "eWorld.h"

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
