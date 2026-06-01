// gcPlayer::BindLocalController(void) static  @ 0x0012111c
// Split-TU isolation: class declared locally to emit the mangled symbol
// __0fIgcPlayerTBindLocalControllervT without touching shared headers.

class gcPlayer {
public:
    static int GetStartingController(void);
    static int BindLocalController(void);
};

// 0x37D884 : int gcPlayer_s_nLocalControllerId[8]
static int *const s_nLocalControllerId = (int *)0x37D884;

int gcPlayer::BindLocalController(void) {
    int ctrl = GetStartingController();
    if (ctrl < 0) {
        return -1;
    }

    int *base = s_nLocalControllerId;
    int i = 0;
    int *p = base;
    do {
        if (*p == ctrl) {
            return -1;
        }
        i++;
        p++;
    } while (i < 8);

    int j = 0;
    do {
        if (*base < 0) {
            *base = ctrl;
            return j;
        }
        j++;
        base++;
    } while (j < 8);

    return -1;
}
