#ifndef ECONTACTCOLLECTOR_H
#define ECONTACTCOLLECTOR_H

class mVec3;
class eContact;

class eContactCollector {
public:
    enum eContactType {};

    void ClearContacts(eContactType);
    void ClearAllContacts(void);
    void FindSlideVelocity(mVec3 *) const;
    int ValidateTri(eContact *, int);
    int ValidatePoint(eContact *, int, int);
    void VerifySoftContacts(void);
};

#endif
