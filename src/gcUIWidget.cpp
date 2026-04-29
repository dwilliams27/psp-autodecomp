#include "gcUIWidget.h"

class cBase;
class cMemPool;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_0009990C;

const cType *gcUIWidget::GetType(void) const {
    if (D_0009990C == 0) {
        if (D_000385E0 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                               &cNamed::New, 0, 0, 0);
        }
        D_0009990C = cType::InitializeType(0, 0, 0x84, D_000385E0,
                                           0, 0, 0, 0);
    }
    return D_0009990C;
}

void gcUIWidget::OnGetSize(mVec2 *, mVec2 *, unsigned int) const {
}
