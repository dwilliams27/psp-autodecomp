// gcViewport — viewport class. Instances live in a contiguous global array
// based at *(0x37D840); each is 0x1390 bytes wide. GetName formats the
// instance's 1-based index into the supplied buffer.

class gcViewport {
public:
    void GetName(char *) const;
};

extern "C" int cStrFormat(char *, const char *, ...);

// ── gcViewport::GetName(char *) const @ 0x002492E8 ──
void gcViewport::GetName(char *buf) const {
    cStrFormat(buf, (const char *)0x36DBCC,
               ((int)((const char *)this - *(char **)0x37D840) / 0x1390) + 1);
}
