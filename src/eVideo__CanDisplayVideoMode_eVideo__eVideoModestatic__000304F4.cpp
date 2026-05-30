// eVideo::CanDisplayVideoMode(eVideo::eVideoMode) static
// Address: 0x000304f4, Size: 56B
// Obj: eAll_psp.obj

class eVideo {
public:
    enum eVideoMode {
        eVM_0 = 0,
        eVM_1 = 1,
        eVM_2 = 2
    };
    static bool CanDisplayVideoMode(eVideoMode mode);
};

bool eVideo::CanDisplayVideoMode(eVideoMode mode)
{
    switch (mode) {
    case 0:
        return true;
    case 1:
        return false;
    case 2:
        return true;
    }
    return false;
}
