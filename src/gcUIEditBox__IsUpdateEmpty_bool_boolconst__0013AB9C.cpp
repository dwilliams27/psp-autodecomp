// gcUIEditBox::IsUpdateEmpty(bool, bool) const @ 0x0013AB9C
// Split-TU: declare the class locally so we don't touch the shared gcUIEditBox.cpp.

class gcUIWidget {
public:
    int IsUpdateEmpty(bool, bool) const;
};

class gcUIEditBox : public gcUIWidget {
public:
    int IsUpdateEmpty(bool, bool) const;
};

int gcUIEditBox::IsUpdateEmpty(bool b, bool TBK) const {
    if (b && *(const int *)((const char *)this + 0x118) > 0) {
        return 0;
    }
    if (!b) {
        unsigned char *base = (unsigned char *)0x454A8;
        if (*(base + 0xA09) != 0) {
            return 0;
        }
    }
    return ((const gcUIWidget *)this)->IsUpdateEmpty(b, TBK);
}
