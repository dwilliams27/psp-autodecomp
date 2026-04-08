#include "eMultiSphereShape.h"
#include "eCapsuleShape.h"
#include "eSimulatedController.h"
#include "eTriangleShape.h"
#include "gcStreamedCinematic.h"

int eMultiSphereShape::CanSweep(void) const {
    return 1;
}

int eCapsuleShape::CanSweep(void) const {
    return 1;
}

int eSimulatedController::GetCollisionMask(void) const {
    return collisionMask;
}

int eTriangleShape::CanSweep(void) const {
    return 0;
}

int gcStreamedCinematic::AllowParentCreate(void) const {
    return 0;
}
