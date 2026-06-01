// gcPlayer::AssignClientPlayerId(nwConnectionHandle, int, const char *, float) static
//   @ 0x001205e8
// Split-TU isolation: class declared locally to emit the mangled symbol
// __0fIgcPlayerUAssignClientPlayerId6SnwConnectionHandleiPCcfT without
// touching shared headers.

// nwConnectionHandle is a named (user-defined) type in the mangling
// (6SnwConnectionHandle), so it must be a distinct type, not a typedef of int.
// Modeled as a 4-byte struct: SNC homes a by-value struct param to the stack,
// matching the original's stack-spill of the handle across the call.
struct nwConnectionHandle { int v; };

struct gcPlayerSlot {
    char pad0C[0xC];      // 0x00
    char name[0x18];      // 0x0C
    int  arg1;            // 0x24
    int  handle;          // 0x28
    char pad2C[0x8];      // 0x2C
    char flag34;          // 0x34
    char flag35;          // 0x35
    char pad36[2];        // 0x36
    float val38;          // 0x38
    char pad3C[0x8];      // 0x3C  (pad to 0x44 stride)
};

class gcPlayer {
public:
    static int FindAvailablePlayer(void);
    static int AssignClientPlayerId(nwConnectionHandle, int, const char *, float);
};

extern "C" char *cStrCopy(char *, const char *, int);

// 0x37D87C : gcPlayer *gcPlayer_s_pPlayers (stride 0x44)
static gcPlayerSlot *const *const s_pPlayers = (gcPlayerSlot *const *)0x37D87C;

int gcPlayer::AssignClientPlayerId(nwConnectionHandle handle, int arg1,
                                   const char *name, float arg3) {
    int index = FindAvailablePlayer();
    if (index < 0) {
        return 0;
    }

    gcPlayerSlot *player = *s_pPlayers + index;
    int *ph = &player->handle;
    *ph = handle.v;
    player->flag34 = 0;
    player->flag35 = 1;
    player->arg1 = arg1;
    cStrCopy(player->name, name, 0x14);
    player->val38 = arg3;
    return 1;
}
