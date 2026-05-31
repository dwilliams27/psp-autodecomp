// gcUIWidgetGroup::DeleteAllWidgets(bool) @ 0x000DF620
// Split-TU: local declarations to match SNC codegen for this object.

struct gcUICell {
    short row;
    short col;
    gcUICell() {}
    gcUICell(const gcUICell &o) : row(o.row), col(o.col) {}
};

class cBase;

class gcUIWidgetGroup {
public:
    void DeleteAllWidgets(bool);
    void DeleteCellWidgets(gcUICell, bool);
};

void gcUIWidgetGroup::DeleteAllWidgets(bool flag) {
    int width = 0;
    const int *widthArray = *(const int *const *)((char *)this + 0xC0);
    const int *heightArray = *(const int *const *)((char *)this + 0xC4);
    if (widthArray != 0) {
        width = widthArray[-1] & 0x3FFFFFFF;
    }

    int height = 0;
    if (heightArray != 0) {
        height = heightArray[-1] & 0x3FFFFFFF;
    }

    int count = width;
    count *= height;
    int index = 0;
    if (index < count) {
        do {
            gcUICell scratch;
            scratch.row = -1;
            scratch.col = -1;
            const int *colArray = *(const int *const *)((char *)this + 0xC4);
            int divisor = 0;
            if (colArray != 0) {
                divisor = colArray[-1] & 0x3FFFFFFF;
            }
            scratch.row = (short)(index / divisor);
            scratch.col = (short)(index % divisor);
            DeleteCellWidgets(scratch, flag);
            index += 1;
        } while (index < count);
    }
}
