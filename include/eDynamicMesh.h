#pragma once

#include "cObject.h"

class cFile;
class cMemPool;
class cName;

// eDynamicMesh: mesh data object (inherits cObject).
// cObject base: offsets 0x00..0x43 (from cObject ctor 0x9d28)
// Struct size: at least 0x84 bytes
class eDynamicMesh : public cObject {
public:
    eDynamicMesh(cBase *);
    ~eDynamicMesh();

    const cType *GetType(void) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void PlatformRead(cFile &, cMemPool *);
    void PlatformFree();
    int GetNodeIndex(const cName &, int) const;
    int GetCollisionShapeIndex(const cName &) const;
    int HasSkin() const;
    void Free();
    void Reset(cMemPool *, bool);

    void AssignCopy(const cBase *);
    static eDynamicMesh *New(cMemPool *, cBase *);
};
