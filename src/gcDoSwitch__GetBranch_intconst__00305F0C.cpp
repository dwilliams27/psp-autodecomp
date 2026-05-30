// gcDoSwitch::GetBranch(int) const  @ 0x00305f0c, 44B, gcAll_psp.obj
// Identical codegen to matched sibling gcDoEvaluation::GetBranch (0x2d89f4).
// Branch payloads at +0x10 (branch 0) and +0x18 (branch 1).

class gcExpression;

class gcDoSwitch {
public:
    gcExpression *GetBranch(int index) const;
};

gcExpression *gcDoSwitch::GetBranch(int index) const {
    int saved = index;
    gcExpression *result;
    if (index == 0) {
        result = *(gcExpression **)((const char *)this + 0x10);
    } else {
        index = 0;
        if (saved == 1) {
            index = (int)*(gcExpression **)((const char *)this + 0x18);
        }
        result = (gcExpression *)index;
    }
    return result;
}
