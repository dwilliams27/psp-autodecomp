// 0x001079f4 — gcGame::FindStreamedCinematicConfigGroups(gcFindStreamedCinematicContext, const cBase *) static
// Split-TU local redeclaration for one gcGame static method.

class cBase;

class gcGame {
public:
    enum gcFindStreamedCinematicContext {};
    static void *FindStreamedCinematicConfigGroups(const cBase *pBase);
    static void *FindStreamedCinematicConfigGroups(gcFindStreamedCinematicContext ctx, const cBase *pBase);
};

void *gcGame::FindStreamedCinematicConfigGroups(gcFindStreamedCinematicContext ctx, const cBase *pBase) {
    if (pBase == 0) {
        return 0;
    }
    switch ((int)ctx) {
        case 0: {
            char *g = *(char **)0x37D854;
            void *r = 0;
            if (g != 0) {
                r = g + 6048;
            }
            return r;
        }
        case 1: {
            char *g = *(char **)0x37D7FC;
            void *r = 0;
            if (g != 0) {
                r = g + 92;
            }
            return r;
        }
        case 2:
            return FindStreamedCinematicConfigGroups(pBase);
        default:
            return 0;
    }
}
