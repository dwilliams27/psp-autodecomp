// gcUIMarqueeControl — UI marquee control, derives from gcUIWidget.

class gcUIWidget {
public:
    bool IsUpdateEmpty(bool, bool) const;
};

class gcUIMarqueeControl : public gcUIWidget {
public:
    bool IsUpdateEmpty(bool, bool) const;
};

// ── gcUIMarqueeControl::IsUpdateEmpty(bool, bool) const @ 0x0013C164 ──
// If `a` is true, the marquee considers itself non-empty. Otherwise it falls
// back to the base widget's IsUpdateEmpty.
bool gcUIMarqueeControl::IsUpdateEmpty(bool a, bool b) const {
    if (a) {
        return false;
    }
    return gcUIWidget::IsUpdateEmpty(a, b);
}
