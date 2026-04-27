// gcEvent::operator=(const gcEvent &) — 0x000d61d0

class gcExpressionList {
public:
    gcExpressionList &operator=(const gcExpressionList &other);
};

class gcEvent {
public:
    char _pad0[8];
    gcExpressionList mExprList;     // +0x08
    char _pad1[4];                  // pad so mField10 lands at +0x10
    int mField10;                   // +0x10

    gcEvent &operator=(const gcEvent &other);
};

gcEvent &gcEvent::operator=(const gcEvent &other) {
    mExprList = other.mExprList;
    mField10 = other.mField10;
    return *this;
}
