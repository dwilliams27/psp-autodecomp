#include "cBase.h"

gcExpression *gcExpression::GetNext(void) const {
    return (gcExpression *)(*(unsigned int *)((char *)this + 8) & ~3u);
}
