// main @ 0x001a984c (gMain_psp.obj)
// C-runtime app entry: calls __snmain(), then gcGameCore::Run(argc, argv),
// returns 0 on success (Run != 0) else -1.

class gcGameCore {
public:
    static int Run(int argc, char **argv);
};

int main(int argc, char **argv) {
    int ret = -1;
    if (gcGameCore::Run(argc, argv) != 0) {
        ret = 0;
    }
    return ret;
}
