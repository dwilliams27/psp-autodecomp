// ODR-WARNING: this TU intentionally redeclares gcUIWidgetList/gcUICell with
// the exact field access used by GetItemFromCell. The method receives the
// wider by-value cell through a hidden pointer, then reads two packed shorts.
struct gcUICell {
    short row;
    short col;
    gcUICell(const gcUICell &);
    ~gcUICell(void);
};

class gcUIWidgetList {
public:
    int GetItemFromCell(gcUICell) const;
};

#pragma control sched=1
int gcUIWidgetList::GetItemFromCell(gcUICell cell) const {
    unsigned int item = *(unsigned int *)((const char *)this + 0xD8);
    int isTagged = item & 1;
    int canUse = 0;
    if (isTagged != 0) {
        canUse = 1;
    }

    int invalid;
    if (canUse != 0) {
        invalid = 1;
    } else {
        invalid = ((item < 1U) & 0xFF) != 0;
        __asm__ volatile("" ::: "memory");
    }

    if (invalid != 0) {
        return -1;
    }

    int row = cell.row;
    int col = cell.col;
    int index;
    __asm__ volatile("" ::: "memory");
    int vertical = (((*(int *)((const char *)this + 0xE4) & 2) != 0) & 0xFF);
    if (vertical != 0) {
        int *cols = *(int **)((const char *)this + 0xC4);
        int count = 0;
        if (cols != 0) {
            count = cols[-1] & 0x3FFFFFFF;
        }
        index = row * count + col;
    } else {
        int *rows = *(int **)((const char *)this + 0xC0);
        int count = 0;
        if (rows != 0) {
            count = rows[-1] & 0x3FFFFFFF;
        }
        index = col * count + row;
    }

    int tagged = 0;
    if (isTagged != 0) {
        tagged = 1;
        __asm__ volatile("" ::: "memory");
    }
    __asm__ volatile("" ::: "memory");
    if (tagged != 0) {
        item = 0;
        __asm__ volatile("" : "+r"(item));
        return *(int *)(item + 0x18) + index;
    }
    __asm__ volatile("" : "+r"(item));
    return *(int *)(item + 0x18) + index;
}
#pragma control sched=2
