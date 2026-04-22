#ifndef EWORLD_H
#define EWORLD_H

struct mVec3;

class eSound {
public:
    char pad_sound[36];
    eSound *nextSound;  // 0x24
    eSound *prevSound;  // 0x28

    void Stop(void);
};

class eRoom {
public:
    char pad_room[264];
    eRoom *prevRoom;    // 0x108
    eRoom *nextRoom;    // 0x10C

    int IsPointInFluidVolume(const mVec3 &) const;
};

class eRoomSet {
public:
    char pad_roomset[80];
    eRoomSet *prevRoomSet;  // 0x50
    eRoomSet *nextRoomSet;  // 0x54
};

class eWorld {
public:
    int field_0x00;
    int field_0x04;
    int field_0x08;
    int field_0x0C;
    eRoom *roomList;       // 0x10
    eRoomSet *roomSetList; // 0x14
    eSound *soundList;     // 0x18
    int field_0x1C;
    int field_0x20;
    mutable int lockCount; // 0x24

    eWorld();
    ~eWorld();
    void LockWorld(bool lock) const;
    void AddSound(eSound *s);
    void AddRoom(eRoom *r);
    void AddRoomSet(eRoomSet *rs);
    void RemoveSound(eSound *s);
    void RemoveRoom(eRoom *r);
    void RemoveRoomSet(eRoomSet *rs);
    const eRoom *GetRoomFromPos(const eRoom *, const mVec3 &) const;
    int IsPointInFluidVolume(const eRoom *, const mVec3 &) const;
    static int GetNextCullId(void);
};

#endif
