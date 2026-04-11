#ifndef GCUI_H
#define GCUI_H

class gcUIDialog;
class gcDoUIShowDialog;

class gcUI {
public:
    void ActualOpenDialog(gcUIDialog *dialog);
    void CloseDialog(gcUIDialog *dialog, const gcDoUIShowDialog *show, float *f);
};

#endif
