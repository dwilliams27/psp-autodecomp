#ifndef ESIMULATEDCONTROLLER_H
#define ESIMULATEDCONTROLLER_H

class eGeom;

class eSimulatedController {
public:
    void InvalidateCacheEntries(eGeom *);
    void RemoveContacts(void);

    char _pad[0x150];
    int contactCount;
};

#endif
