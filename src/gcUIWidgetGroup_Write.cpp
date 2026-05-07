// ODR-WARNING: split-TU local redeclaration for one method only.
// Keep this class shape minimal so matched siblings in gcUIWidgetGroup.cpp do not drift.
class cBase;
class cFile;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(unsigned int);
    void Write(int, const float *);
    void WriteBase(const cBase *);
    void End(void);
};

class cHandle {
public:
    int _data;
    void Write(cWriteBlock &) const;
};

class gcUIWidget {
public:
    void Write(cFile &) const;
};

class gcUIWidgetGroup : public gcUIWidget {
public:
    void Write(cFile &) const;
};

void gcUIWidgetGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 10);
    ((const gcUIWidget *)this)->Write(file);
    wb.Write(*(const unsigned int *)((const char *)this + 0xB0));

    int width = 0;
    const int *widths = *(const int *const *)((const char *)this + 0xC0);
    if (widths != 0) {
        width = widths[-1] & 0x3FFFFFFF;
    }
    wb.Write(width);

    int height = 0;
    const int *heights = *(const int *const *)((const char *)this + 0xC4);
    if (heights != 0) {
        height = heights[-1] & 0x3FFFFFFF;
    }
    wb.Write(height);

    const int *cellWidths = *(const int *const *)((const char *)this + 0xC0);
    register int widthCount asm("s1");
    __asm__ volatile("ori %0,$0,0" : "=r"(widthCount));
    const int *cellHeights =
        *(const int *const *)((const char *)this + 0xC4);
    if (cellWidths != 0) {
        widthCount = cellWidths[-1] & 0x3FFFFFFF;
    }

    int heightCount = 0;
    if (cellHeights != 0) {
        heightCount = cellHeights[-1] & 0x3FFFFFFF;
    }

    int cellCount = widthCount;
    cellCount *= heightCount;
    int cell = 0;
    if (cell < cellCount) {
        const char *heads = *(const char *const *)((const char *)this + 0xBC);
        int offset = 0;
        do {
            const cBase *head = *(const cBase *const *)(heads + offset);
            wb.WriteBase(head);

            heads = *(const char *const *)((const char *)this + 0xBC);
            const gcUIWidget *widget =
                *(const gcUIWidget *const *)(heads + offset);
            if (widget != 0) {
                const gcUIWidget *first =
                    *(const gcUIWidget *const *)(heads + offset);
                do {
                    const gcUIWidget *next =
                        *(const gcUIWidget *const *)((const char *)widget +
                                                     0x68);
                    const cBase *writePtr = 0;
                    if (next != first) {
                        writePtr = (const cBase *)next;
                    }
                    wb.WriteBase(writePtr);
                    heads = *(const char *const *)((const char *)this + 0xBC);
                    widget =
                        *(const gcUIWidget *const *)((const char *)widget +
                                                     0x68);
                    first = *(const gcUIWidget *const *)(heads + offset);
                } while (widget !=
                         first);
            }
            cell += 1;
            offset += 4;
            heads = *(const char *const *)((const char *)this + 0xBC);
        } while (cell < cellCount);
    }

    const int *spriteArray = *(const int *const *)((const char *)this + 0xC8);
    int spriteCount = 0;
    if (spriteArray != 0) {
        spriteCount = spriteArray[-1] & 0x3FFFFFFF;
    }
    wb.Write(spriteCount);

    const cHandle *sprites =
        *(const cHandle *const *)((const char *)this + 0xC8);
    int count = 0;
    if (sprites != 0) {
        count = ((const int *)sprites)[-1] & 0x3FFFFFFF;
    }

    int i = 0;
    if (i < count) {
        register int handleOffset asm("a0") = 0;
        __asm__ volatile("ori %0,$0,0" : "=r"(handleOffset));
        const cHandle *handle =
            (const cHandle *)((const char *)sprites + handleOffset);
        do {
            handle->Write(wb);
            i += 1;
            handle = (const cHandle *)((const char *)handle + 4);
        } while (i < count);
    }

    wb.Write(2, (const float *)((const char *)this + 0xB4));
    wb.Write(2, (const float *)((const char *)this + 0xCC));
    wb.End();
}
