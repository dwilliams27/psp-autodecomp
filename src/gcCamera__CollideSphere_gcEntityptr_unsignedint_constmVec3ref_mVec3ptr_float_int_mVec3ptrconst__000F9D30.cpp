// gcCamera::CollideSphere(gcEntity*, unsigned int, const mVec3&, mVec3*, float, int, mVec3*) const
// @ 0x000f9d30  symbol __0fIgcCameraNCollideSphereP6IgcEntityUiRC6FmVec3P6FmVec3fiTEK
// Split-TU: classes declared locally to avoid touching shared headers.

typedef unsigned int SceULong128 __attribute__((mode(TI)));

struct mVec3 {
    SceULong128 qw;
};

// eSphereCast: stack-built collision query object. Layout: result vectors live
// at +0x30 (position) and +0x40 (normal); a hit flag at the int slot.
class eDynamicGeom;
class eWorld;
class eRoom;

class eSphereCast {
public:
    SceULong128 mPad0[3];    // 0x00..0x2F
    mVec3       mPos;        // 0x30
    mVec3       mNormal;     // 0x40
    SceULong128 mPad1[10];   // 0x50..0xEF  (object is 0xF0 bytes total)
    eSphereCast(const mVec3 &origin, const mVec3 &end, float radius,
                unsigned int mask, int flags);
    void AddIgnoreGeom(const eDynamicGeom *geom);
    int  Intersect(const eWorld *world, const eRoom *room);
};

class gcEntity {
public:
    char        pad0[0x218];
    gcEntity   *mNext;       // 0x218
};

class gcCamera {
public:
    char        pad[0x1124];
    eWorld     *mWorld;      // 0x1124
    eRoom      *mRoom;       // 0x1128
    int CollideSphere(gcEntity *ent, unsigned int flags, const mVec3 &center,
                      mVec3 *outPos, float radius, int wantNormal, mVec3 *outNormal) const;
};

int gcCamera::CollideSphere(gcEntity *ent, unsigned int flags, const mVec3 &center,
                            mVec3 *outPos, float radius, int wantNormal,
                            mVec3 *outNormal) const
{
    if (flags == 0)
        return 0;

    eSphereCast cast(center, *outPos, radius, flags, wantNormal);

    while (ent != 0) {
        cast.AddIgnoreGeom((const eDynamicGeom *)((char *)ent + 0x80));
        ent = ent->mNext;
    }

    if (cast.Intersect(mWorld, mRoom) != 0) {
        *outPos = cast.mPos;
        if (outNormal != 0)
            *outNormal = cast.mNormal;
        return 1;
    }
    return 0;
}
