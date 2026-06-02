// gcDoLobbyMailOp::Evaluate(void) const
// Address: 0x002e19d8, Size: 404B
// Emits: __0fPgcDoLobbyMailOpIEvaluatevK

// Dispatch-table entry: {short adj; void *fn}. The lobby singleton's first
// word points at a table of these; each "method" is fn(lobby + adj, ...).
typedef float (*gcExprVFn)(const void *);
struct gcExprVEntry {
    short adj;
    short pad;
    gcExprVFn fn;
};

class gcDoLobbyMailOp {
public:
    char base[0x0C];
    int mMode;   // 0x0C
    int mArg10;  // 0x10
    int mVal14;  // 0x14
    float Evaluate(void) const;
};

extern void **nwNetwork__GetLobby();

float gcDoLobbyMailOp::Evaluate(void) const {
    char mail[284];
    void **lobby = nwNetwork__GetLobby();
    if (lobby != 0) {
        char *vtab = *(char **)lobby;
        int val = this->mVal14;

        int hasTag = 0;
        const gcExprVEntry *e0 = (const gcExprVEntry *)(vtab + 0x470);
        void *dst0 = (char *)lobby + e0->adj;

        int tag = val & 1;
        if (tag != 0) {
            hasTag = 1;
        }
        const void *child;
        if (hasTag != 0) {
            child = 0;
        } else {
            child = (const void *)val;
        }
        const void *node = child;

        float fval;
        if (node != 0) {
            const gcExprVEntry *ec = (const gcExprVEntry *)((char *)((void **)node)[1] + 0x70);
            fval = ec->fn((const char *)node + ec->adj);
        } else {
            fval = 0.0f;
        }

        ((void (*)(void *, int, void *))e0->fn)(dst0, (int)fval, (void *)mail);

        switch (this->mMode) {
        case 0: {
            const gcExprVEntry *e = (const gcExprVEntry *)(*(char **)lobby + 0x480);
            ((void (*)(void *, void *))e->fn)((char *)lobby + e->adj, (void *)mail);
            break;
        }
        case 1: {
            const gcExprVEntry *e = (const gcExprVEntry *)(*(char **)lobby + 0x488);
            ((void (*)(void *, void *, int))e->fn)((char *)lobby + e->adj, (void *)mail, 0);
            break;
        }
        case 2: {
            const gcExprVEntry *e = (const gcExprVEntry *)(*(char **)lobby + 0x488);
            ((void (*)(void *, void *, int))e->fn)((char *)lobby + e->adj, (void *)mail, 1);
            break;
        }
        case 3: {
            const gcExprVEntry *e = (const gcExprVEntry *)(*(char **)lobby + 0x490);
            ((void (*)(void *, void *, int))e->fn)((char *)lobby + e->adj, (void *)mail, this->mArg10);
            break;
        }
        }
    }
    return 0.0f;
}
