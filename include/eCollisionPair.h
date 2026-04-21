#pragma once

#include "eCollisionConstraint.h"
class eCollisionContactInfo;
class eConsolidatedContact;
class eConstraintSolver;
class eSurface;
class mVec3;
class cName;

// Forward declare body type. Bodies are accessed at +0x10 and +0x14.
// BodiesSleeping (0x3417c) checks:
//   body = this[0x10]; flag = *(unsigned short*)(body + 0x98) & 0x40
//   body = this[0x14]; flag = *(unsigned short*)(body + 0x98) & 0x40
// RemoveContacts (0x34130) shows:
//   +0x48: linked list head of eCollisionConstraint*
//   +0x42: unsigned short contact count
class eRigidBody;

class eCollisionPair {
public:
    char _pad00[0x10];                      // 0x00
    eRigidBody *mBodyA;                     // 0x10
    eRigidBody *mBodyB;                     // 0x14
    char _pad18[0x2A];                      // 0x18
    unsigned short mContactCount;           // 0x42
    char _pad44[0x04];                      // 0x44
    eCollisionConstraint *mContactList;     // 0x48
    char _pad4C[0x18];                      // 0x4C — padding to align cRedBlackTreeNode fields

    void RemoveContacts();
    int BodiesSleeping() const;
    int IsDisjoint() const;
    eCollisionConstraint *FindContact(const eContactFeature &) const;
    void CleanupContacts();
    int AddContact(eCollisionConstraint *);
    void Collide(unsigned short, eConstraintSolver *);
    void AddExternalContact(const mVec3 &, const mVec3 &, eConstraintSolver *);
    int HasMeshShape();
    int GetBodyASurface(int) const;
    int GetBodyBSurface(int) const;
    void WakeUpPair();
    void ReportContacts(const eConsolidatedContact *, int, const eSurface *, const eSurface *) const;
    void HandleContacts(const eCollisionContactInfo &, eConstraintSolver *, unsigned int);
    void CollideDynamicMesh(eConstraintSolver *);
    int CanCollide();
};
