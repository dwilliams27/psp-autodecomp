// 0x0014ff20, 240B  gcDoSwitchCase::Evaluate(void) const
// Split-TU: declare just what we need locally; do NOT touch shared TUs.

class cBase;

struct DispatchEntry {
    short offset;
    short _pad;
    float (*func)(char *);
};

class gcExpressionList {
public:
    float Evaluate(void) const;
};

class gcDoSwitchCase {
public:
    float Evaluate(void) const;
};

extern float D_37D8C4;

float gcDoSwitchCase::Evaluate(void) const {
    int i = 0;
    int off = 0;
    do {
        char *child = *(char **)(*(char **)((char *)this + 0xC) + off);
        if (child == 0) {
            return 0.0f;
        }
        DispatchEntry *e = (DispatchEntry *)(((char **)child)[1] + 0x70);
        float threshold = D_37D8C4;
        short slot = e->offset;
        float (*fn)(char *) = e->func;
        if (threshold == fn(child + slot)) {
            ((const gcExpressionList *)((const char *)this + 0x14))->Evaluate();
            return 1.0f;
        }
        i++;
        off += 4;
    } while (i < 3);
    return 0.0f;
}
