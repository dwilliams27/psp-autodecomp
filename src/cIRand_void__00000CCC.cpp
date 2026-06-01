// cIRand(void) @ 0x00000ccc  symbol __0FGcIRandv
// PRNG used by cNamed::GenerateName, cGUID, cManager::Add, gcGame.

class cDateTime {
public:
    int value;
    explicit cDateTime(int v) : value(v) {}
    static cDateTime Now(void);
};

static int g_irand_init;
static int g_irand_seed;
extern int g_irand_counter;

#pragma control sched=2
int cIRand(void) {
    if (g_irand_init == 0) {
        g_irand_init = 1;
        cDateTime now(cDateTime::Now().value);
        g_irand_seed = now.value;
    }
    int seed = g_irand_seed;
    int *pc = &g_irand_counter;
    int v245 = *pc;
    *pc = v245 + 1;


    int x = ((seed + v245) * 59) + seed;
    x = (x << 13) ^ x;

    return (x * (((x * x) * 15731) + 0xC10E1)) + (int)(0x5208F10FU / (unsigned)(((unsigned)g_irand_counter & 0xFFFFF) | 1));
}
