#ifndef EBOXSHAPE_H
#define EBOXSHAPE_H

class eBoxShape {
public:
    char _pad[0x80];       // 0x00
    float mHalfExtents[4]; // 0x80 — quad-aligned (x,y,z,_)

    float GetVolume(void) const;
};

#endif
