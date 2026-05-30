// eDynamicLight::eDynamicLight(cBase*) @ 0x00077874
// Standard ctor: chain to eDynamicGeom base ctor, set vtable ptr at offset 4.
// Split-TU: declare classes locally.

class cBase;

class eDynamicGeom {
public:
    eDynamicGeom(cBase *parent);
};

class eDynamicLight : public eDynamicGeom {
public:
    eDynamicLight(cBase *parent);
};

extern char eDynamicLightvtbl[];

eDynamicLight::eDynamicLight(cBase *parent) : eDynamicGeom(parent) {
    ((void **)this)[1] = eDynamicLightvtbl;
}
