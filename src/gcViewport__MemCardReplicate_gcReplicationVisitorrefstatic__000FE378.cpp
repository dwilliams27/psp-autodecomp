// 0x000fe378 (268B) — gcViewport::MemCardReplicate(gcReplicationVisitor &) static
// Obj: gcAll_psp.obj
//
// Split-TU: gcViewport / gcUI / gcCamera declared locally. The global gcUI
// singleton lives at 0x99928; viewport instances form a contiguous array
// based at *(0x37D840), each 0x1390 bytes wide, with the gcCamera at +0x10.
//
// The visitor's mode word (offset 0): bit 0 => writing, bit 1 => reading.

class cOutStream;
class cInStream;

class gcReplicationVisitor {
public:
    int mMode;              // 0x00
    cOutStream *mOutStream; // 0x04
    cInStream *mInStream;   // 0x08
};

class gcUI {
public:
    void Write(cOutStream &) const;
    void Read(cInStream &);
};

class gcCamera {
public:
    void Write(cOutStream &) const;
    void Read(cInStream &);
};

class gcViewport {
public:
    static void MemCardReplicate(gcReplicationVisitor &);
};

extern char *gViewportArrayBase; // 0x37D840

static inline bool TestBit(int v) { return v != 0; }

void gcViewport::MemCardReplicate(gcReplicationVisitor &visitor) {
    int mode = visitor.mMode;
    int wbit = mode & 1;
    int writing = 0;
    if (wbit) {
        writing = 1;
    }
    if (writing) {
        ((gcUI *)0x99928)->Write(*visitor.mOutStream);
    } else if (TestBit(mode & 2)) {
        ((gcUI *)0x99928)->Read(*visitor.mInStream);
    }

    int i = 0;
    int off = 0;
    do {
        int m = visitor.mMode;
        int wb = m & 1;
        int w = 0;
        if (wb) {
            w = 1;
        }
        if (w) {
            ((gcCamera *)(gViewportArrayBase + off + 0x10))->Write(*visitor.mOutStream);
        } else if (TestBit(m & 2)) {
            ((gcCamera *)(gViewportArrayBase + off + 0x10))->Read(*visitor.mInStream);
        }
        i += 1;
        off += 0x1390;
    } while (i < 5);
}
