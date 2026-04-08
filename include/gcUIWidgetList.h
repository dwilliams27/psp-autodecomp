#ifndef GCUIWIDGETLIST_H
#define GCUIWIDGETLIST_H

struct gcUICell {
    int row;
    int col;
};

class gcUIWidgetList {
public:
    void FillCell(gcUICell, int);
};

#endif
