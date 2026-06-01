// eMoviePlatform::AllocWorkArea(int, int) @ 0x00011bdc
// symbol: __0fOeMoviePlatformNAllocWorkAreaiTB

#pragma control sched=2

class cVolatile {
public:
    static int Alloc(unsigned int size);
};

class eMoviePlatform {
public:
    unsigned int base;   // 0x0
    unsigned int top;    // 0x4
    unsigned int limit;  // 0x8
    int AllocWorkArea(int size, int extra);
};

int eMoviePlatform::AllocWorkArea(int size, int extra)
{
    int useVolatile = (*(int *)0x37BC34 != 0) & 0xFF;
    int result = 0;

    if (useVolatile) {
        result = cVolatile::Alloc((unsigned int)extra);
    } else {
        if (size < 4) {
            size = 4;
        }

        unsigned int align = ((unsigned int)(size + 3) >> 2) << 2;
        unsigned int alignM1 = align - 1;
        unsigned int rounded = (unsigned int)((top + alignM1) / align) * align;
        unsigned int newTop = rounded + (unsigned int)extra;

        if (limit >= newTop - base) {
            result = (int)rounded;
            top = newTop;
        }
    }
    return result;
}
