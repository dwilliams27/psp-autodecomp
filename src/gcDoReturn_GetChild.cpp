#include "gcDoReturn.h"
#include "cBase.h"

gcExpression *gcDoReturn::GetChild(int index) const {
    gcExpression *result = 0;
    if (index == 0) {
        int val = ((int *)this)[3];
        int flag = 0;
        if (val & 1) {
            flag = 1;
        }
        gcExpression *ptr;
        if (flag != 0) {
            ptr = 0;
        } else {
            ptr = (gcExpression *)val;
        }
        result = ptr;
    }
    return result;
}
