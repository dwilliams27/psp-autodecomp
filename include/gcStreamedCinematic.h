#ifndef GCSTREAMEDCINEMATIC_H
#define GCSTREAMEDCINEMATIC_H

class gcCinematicInstance;

class gcStreamedCinematic {
public:
    char pad_00[0xCC];
    gcCinematicInstance *mpCinematicInstance; // 0xCC

    int AllowParentCreate(void) const;
    void NeedsDelete(void);
};

#endif
