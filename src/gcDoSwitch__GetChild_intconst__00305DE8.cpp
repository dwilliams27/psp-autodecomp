// gcDoSwitch::GetChild(int) const  @ 0x00305de8
// Sibling-identical to gcDoReturn::GetChild (matched). The (unkC&1)?0:unkC
// idiom with the conditional flag produces the SNC branch-likely pattern-5
// bnezl at the andi result. Class declared locally (split-TU) because
// include/gcDoReturn.h must not gain a new GetChild declaration.

class gcExpression;

class gcDoSwitch {
public:
    gcExpression *GetChild(int) const;
};

gcExpression *gcDoSwitch::GetChild(int index) const {
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
