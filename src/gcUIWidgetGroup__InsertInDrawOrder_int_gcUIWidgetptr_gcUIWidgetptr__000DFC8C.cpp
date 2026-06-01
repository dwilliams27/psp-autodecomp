// gcUIWidgetGroup::InsertInDrawOrder(int, gcUIWidget*, gcUIWidget*)
// 0x000DFC8C, gcAll_psp.obj
// Split-TU local declarations to control codegen.

struct gcUIWidget {
    void           *owner;        // 0x00
    char            pad4[0x64];   // 0x04
    gcUIWidget     *prev;         // 0x68 (104)
    gcUIWidget     *next;         // 0x6C (108)
    void Detach();
};

struct gcUIWidgetGroup {
    char            padBC[0xBC];  // 0x00
    gcUIWidget    **draw;         // 0xBC (188)
    void InsertInDrawOrder(int index, gcUIWidget *widget, gcUIWidget *anchor);
};

void gcUIWidgetGroup::InsertInDrawOrder(int index, gcUIWidget *widget, gcUIWidget *anchor)
{
    if (widget == anchor)
        return;

    widget->Detach();

    if (anchor != 0) {
        widget->next = anchor;
        widget->prev = anchor->prev;
        anchor->prev = widget;
        widget->prev->next = widget;
    } else {
        gcUIWidget **slot = draw + index;
        gcUIWidget *head = *slot;
        if (head != 0) {
            widget->next = head->next;
            widget->prev = *(draw + index);
            widget->next->prev = widget;
            widget->prev->next = widget;
        } else {
            *slot = widget;
            widget->prev = widget;
            widget->next = widget;
        }
    }

    widget->owner = (void *)this;
}
