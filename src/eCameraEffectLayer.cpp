#include "eCameraEffectLayer.h"
#include "cBase.h"

inline void *operator new(unsigned int, void *p) { return p; }

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *, cBase *(*)(cMemPool *, cBase *), const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(int);
    void End(void);
};

template <class T> T *dcast(const cBase *);

extern const char eCameraEffectLayer_base_name[];
extern const char eCameraEffectLayer_base_desc[];

static cType *type_base;
static cType *type_eCameraEffectLayer;

// AssignCopy (0x001e2808)
void eCameraEffectLayer::AssignCopy(const cBase *other) {
    eCameraEffectLayer *p = dcast<eCameraEffectLayer>(other);
    mField8 = p->mField8;
    mFieldC = p->mFieldC;
    mField10 = p->mField10;
}

// Write (0x0002cfec)
void eCameraEffectLayer::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    wb.Write(mFieldC);
    mField10.Write(wb);
    wb.Write(mField8);
    wb.End();
}

// GetType (0x001e28e4)
const cType *eCameraEffectLayer::GetType(void) const {
    if (!type_eCameraEffectLayer) {
        if (!type_base) {
            type_base = cType::InitializeType(eCameraEffectLayer_base_name, eCameraEffectLayer_base_desc, 1, 0, 0, 0, 0, 0);
        }
        type_eCameraEffectLayer = cType::InitializeType(0, 0, 0x2B, type_base, &eCameraEffectLayer::New, 0, 0, 0);
    }
    return type_eCameraEffectLayer;
}
