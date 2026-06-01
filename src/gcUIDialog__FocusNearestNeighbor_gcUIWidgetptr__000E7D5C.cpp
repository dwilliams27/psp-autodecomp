// gcUIDialog::FocusNearestNeighbor(gcUIWidget *) @ 0x000E7D5C
// Split-TU: classes declared locally to avoid touching shared headers.

class gcUIWidget {
public:
    enum gcUINeighbor {};
};

class gcUIDialog {
public:
    bool FocusNextControl(gcUIWidget::gcUINeighbor dir, gcUIWidget *w);
    bool FocusNearestNeighbor(gcUIWidget *w);
};

bool gcUIDialog::FocusNearestNeighbor(gcUIWidget *w) {
    if (FocusNextControl((gcUIWidget::gcUINeighbor)3, w)) {
        return true;
    }
    if (FocusNextControl((gcUIWidget::gcUINeighbor)2, w)) {
        return true;
    }
    if (FocusNextControl((gcUIWidget::gcUINeighbor)1, w)) {
        return true;
    }
    if (FocusNextControl((gcUIWidget::gcUINeighbor)0, w)) {
        return true;
    }
    return false;
}
