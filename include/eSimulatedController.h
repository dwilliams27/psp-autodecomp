#ifndef ESIMULATEDCONTROLLER_H
#define ESIMULATEDCONTROLLER_H

class eGeom;

class eSimulatedController {
public:
    void InvalidateCacheEntries(eGeom *);
    void RemoveContacts(void);
    int GetCollisionMask(void) const;

    char _pad[0x34];
    int collisionMask;
    char _pad2[0x118];
    int contactCount;
};

#endif
