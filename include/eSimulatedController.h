#ifndef ESIMULATEDCONTROLLER_H
#define ESIMULATEDCONTROLLER_H

struct mVec3;

class cBase;
class eGeom;
class cFile;
class cMemPool;
class cType;

class ePhysicsController {
public:
    ePhysicsController(cBase *);
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;

    char _basePad[0x10];
};

class eSimulatedController : public ePhysicsController {
public:
    eSimulatedController(cBase *);
    void InvalidateCacheEntries(eGeom *);
    void RemoveContacts(void);
    int GetCollisionMask(void) const;
    void GetVelocity(int index, const mVec3 &pos, mVec3 *out) const;
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    const cType *GetType(void) const;

    char _pad[0x24];
    int collisionMask;      // offset 0x34
    void *bodyEntries;      // offset 0x38
    char _pad2[0x114];      // offset 0x3C
    int contactCount;       // offset 0x150
};

#endif
