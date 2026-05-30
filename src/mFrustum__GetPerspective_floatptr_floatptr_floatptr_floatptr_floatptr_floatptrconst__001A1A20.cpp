#include "mFrustum.h"

void mFrustum::GetPerspective(float *p1, float *p2, float *p3,
                              float *p4, float *p5, float *p6) const {
    const mPlane *q2 = &planes[2];
    const mPlane *q3 = &planes[3];
    *p1 = planes[0].w;
    const mPlane *q1 = &planes[1];
    *p2 = -q1->w;
    *p3 = -(q2->y * *p1) / q2->x;
    const mPlane *q5 = &planes[5];
    const mPlane *q4 = &planes[4];
    *p4 = -(q3->y * *p1) / q3->x;
    *p5 = -(q4->y * *p1) / q4->z;
    *p6 = -(q5->y * *p1) / q5->z;
}
