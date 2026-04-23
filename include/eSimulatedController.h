#ifndef ESIMULATEDCONTROLLER_H
#define ESIMULATEDCONTROLLER_H

struct mVec3;

class eGeom;
class cFile;

class eSimulatedController {
public:
    void InvalidateCacheEntries(eGeom *);
    void RemoveContacts(void);
    int GetCollisionMask(void) const;
    void GetVelocity(int index, const mVec3 &pos, mVec3 *out) const;
    void Write(cFile &) const;

    char _pad[0x34];
    int collisionMask;      // offset 0x34
    void *bodyEntries;      // offset 0x38
    char _pad2[0x114];      // offset 0x3C
    int contactCount;       // offset 0x150
};

#endif
