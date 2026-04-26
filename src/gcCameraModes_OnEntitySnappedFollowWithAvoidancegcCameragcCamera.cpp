class cOutStream;
class eCollisionContactInfo;
class eGeom;
class eHeightmapShape;
class mOCS;

extern "C" int time(int *);

class cTimeValue {
public:
    int value;

    void Write(cOutStream &) const;
};

class cDateTime {
public:
    int value;

    explicit cDateTime(int v) : value(v) {}

    static cDateTime Now(void);
};

class cOutStream {
public:
    void Write(int, int, bool);
};

class gcEntity;

template <class T>
class cHandleT {
public:
    int handle;
};

class gcCamera {
public:
    class gcState {
    public:
        char pad[0xA0];
        cHandleT<gcEntity> followEntity;
    };

    void FollowWithAvoidance(cTimeValue, cHandleT<gcEntity>);
};

class gcCameraModes {
public:
    static void OnEntitySnappedFollowWithAvoidance(gcCamera &, gcCamera::gcState &);
};

class eWorld {
public:
    enum eWorldGeomBin {
        eWorldGeomBin_Decal = 2
    };

    void AddBinGeom(eGeom *, eWorldGeomBin);
};

class eGeom {
public:
    int base;
};

class eDecalSystem : public eGeom {
public:
    char pad[0x60];
    eWorld *world;

    void OnAddedToWorld(void);
};

class eConvexHullShape {
public:
    int Collide(const eHeightmapShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
};

class eCollision {
public:
    static int ConvexHullHeightmap(const eConvexHullShape &, const eHeightmapShape &, int, const mOCS &, const mOCS &, eCollisionContactInfo *);
};

void gcCameraModes::OnEntitySnappedFollowWithAvoidance(gcCamera &camera, gcCamera::gcState &state) {
    camera.FollowWithAvoidance(cTimeValue(), state.followEntity);
}

void eDecalSystem::OnAddedToWorld(void) {
    world->AddBinGeom((eGeom *)this, eWorld::eWorldGeomBin_Decal);
}

cDateTime cDateTime::Now(void) {
    int now;
    time(&now);
    return cDateTime(now);
}

#pragma control sched=1

int eConvexHullShape::Collide(const eHeightmapShape *shape, int, int b, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::ConvexHullHeightmap(*this, *shape, b, ocs1, ocs2, info);
}

#pragma control sched=2

void cTimeValue::Write(cOutStream &out) const {
    out.Write(value, 0x20, true);
}
