// ODR-WARNING: this TU intentionally redeclares gcUIWidgetGroup with a
// minimal member list for Draw only. Do not include gcUIWidgetGroup.h here;
// adding this large method to the canonical TU/header can perturb already
// matched gcUIWidgetGroup siblings.

typedef unsigned int SceULong128 __attribute__((mode(TI)));

struct mOCS {
    SceULong128 row0;
    SceULong128 row1;
    SceULong128 row2;
    SceULong128 row3;
};

struct mVec2 {
    float x;
    float y;
};

struct eRect {
    float x;
    float y;
    float w;
    float h;
};

struct eViewport {
    int x;
    int y;
    int w;
    int h;
};

class eCamera;
class eSprite;

class eVideo {
public:
    static void GetScissor(eViewport *);
    static void SetScissor(const eViewport *);
};

extern "C" void eSprite_GetScreenPos(
    const eCamera *, int, const mVec2 &, const mVec2 &, mVec2 *,
    const eRect *) __asm__(
    "__0fHeSpriteMGetScreenPosPC6HeCamera65HeSpriteT__5HeSpriteHeOriginRC6FmVec2TDP6FmVec2PC6FeRectT");

extern "C" void gcUIWidget_DrawResizable(
    const eCamera &, const mOCS &, const eRect &, const eRect &, const void *,
    float, float, float, bool) __asm__(
    "__0fKgcUIWidgetNDrawResizableRC6HeCameraRC6EmOCSRC6FeRectTDRC6GcArray76IcHandleT76HeSprite__fNCGbT");

class gcUIWidget {
public:
    void GetSize(mVec2 *, mVec2 *, unsigned int) const;
    void MouseOver(const eRect &);
    static void GetScissor(const eCamera &, const eRect &, eRect *);
};

struct DispatchEntry {
    short offset;
    short pad;
    void *fn;
};

struct gcUICell {
    short row;
    short col;
};

class gcUIWidgetGroup : public gcUIWidget {
public:
    void ResizeCellsEvenly(const eRect &, unsigned int) const;
    void Draw(const eCamera &, const mOCS &, const eRect &, const eRect &,
              float, float, float, unsigned int);
};

static int array_count(const void *arr) {
    int count = 0;
    if (arr != 0) {
        count = *(const int *)((const char *)arr - 4) & 0x3FFFFFFF;
    }
    return count;
}

void gcUIWidgetGroup::Draw(const eCamera &camera, const mOCS &ocs,
                           const eRect &rect, const eRect &clip, float alpha,
                           float scale, float angle, unsigned int flags) {
    int forceDraw = flags & 2;
    if (forceDraw == 0) {
        int needsEffect =
            ((*(unsigned int *)((char *)this + 0x24) & 0x20000000) != 0) &
            0xFF;
        if (needsEffect == 0) {
            return;
        }
    }

    float drawAlpha =
        (*(float *)((char *)this + 0x80) * *(float *)((char *)this + 0x84)) *
        alpha;
    if (drawAlpha == 0.0f) {
        return;
    }

    float drawScale =
        *(float *)((char *)this + 0x88) * *(float *)((char *)this + 0x8C);
    if (((*(unsigned int *)((char *)this + 0x24) & 0x80) != 0) & 0xFF) {
        scale = 1.0f;
    }
    drawScale = drawScale * scale;
    if (drawScale == 0.0f) {
        return;
    }

    unsigned int groupFlags = *(unsigned int *)((char *)this + 0xB0);
    if (((groupFlags & 0x10000000) != 0) & 0xFF) {
        int canResize = 1;
        if ((groupFlags & 1) != 0) {
            canResize = 0;
        }
        if (((canResize != 0) || !(((groupFlags & 0x20) != 0) & 0xFF)) &&
            !(((groupFlags & 0x10) != 0) & 0xFF)) {
            ResizeCellsEvenly(rect, flags);
        }
    }

    mVec2 pos0;
    mVec2 pos1;
    mVec2 pos2;
    mVec2 pos3;
    DispatchEntry *screenEntry =
        (DispatchEntry *)(*(char **)((char *)this + 4) + 0xF8);
    typedef void (*ScreenFn)(void *, const eCamera *, const eRect &,
                             unsigned int, mVec2 *, mVec2 *, mVec2 *,
                             mVec2 *);
    ((ScreenFn)screenEntry->fn)((char *)this + screenEntry->offset, &camera,
                                rect, flags, &pos0, &pos1, &pos2, &pos3);

    eRect mouseRect;
    mouseRect.x = pos0.x;
    mouseRect.y = pos0.y;
    mouseRect.w = pos1.x;
    mouseRect.h = pos1.y;
    ((gcUIWidget *)this)->MouseOver(mouseRect);

    unsigned int widgetFlags = *(unsigned int *)((char *)this + 0x24);
    float drawAngle = *(float *)((char *)this + 0x7C) + angle;
    if (180.0f < drawAngle) {
        drawAngle = drawAngle - 360.0f;
    } else if (drawAngle < -180.0f) {
        drawAngle = angle + *(float *)((char *)this + 0x7C) + 360.0f;
    }

    if (((widgetFlags & 0x100) != 0) & 0xFF) {
        int drawResizable = 0;
        if (((widgetFlags & 4) != 0) & 0xFF) {
            if (((widgetFlags & 0x400) != 0) & 0xFF) {
                drawResizable = 1;
            }
        }
        if (drawResizable == 0) {
            if (((widgetFlags & 0x04000000) != 0) & 0xFF) {
                if (((widgetFlags & 0x800) != 0) & 0xFF) {
                    drawResizable = 1;
                }
            }
        }
        if (drawResizable == 0) {
            if (!(((widgetFlags & 0x400) != 0) & 0xFF) &&
                !(((widgetFlags & 0x800) != 0) & 0xFF)) {
                drawResizable = 1;
            }
        }
        if (drawResizable != 0) {
            eRect drawRect;
            drawRect.x = pos0.x;
            drawRect.y = pos0.y;
            drawRect.w = pos1.x;
            drawRect.h = pos1.y;
            eRect drawClip;
            drawClip.x = pos2.x;
            drawClip.y = pos2.y;
            drawClip.w = pos3.x;
            drawClip.h = pos3.y;

            DispatchEntry *spritesEntry =
                (DispatchEntry *)(*(char **)((char *)this + 4) + 0x80);
            typedef void *(*SpritesFn)(void *);
            void *sprites =
                ((SpritesFn)spritesEntry->fn)((char *)this + spritesEntry->offset);
            gcUIWidget_DrawResizable(camera, ocs, drawRect, drawClip, sprites,
                                     drawAlpha, drawScale, drawAngle,
                                     ((widgetFlags & 0x200) != 0) & 0xFF);
        }
    }

    if (forceDraw != 0) {
        mVec2 unused;
        ((gcUIWidget *)this)->GetSize(&pos1, &unused, flags | 1);
    }

    eRect drawArea;
    drawArea.x = pos0.x;
    drawArea.y = pos0.y;
    drawArea.w = pos1.x;
    drawArea.h = pos1.y;

    eRect drawClip;
    drawClip.x = clip.x;
    drawClip.y = clip.y;
    drawClip.w = clip.w;
    drawClip.h = clip.h;

    eViewport oldScissor;
    if (((*(unsigned int *)((char *)this + 0xB0) & 0x10) != 0) & 0xFF) {
        eRect scissorRect;
        gcUIWidget::GetScissor(camera, drawArea, &scissorRect);
        eViewport newScissor;
        newScissor.x = (int)scissorRect.x;
        newScissor.y = (int)scissorRect.y;
        newScissor.w = (int)scissorRect.w;
        newScissor.h = (int)scissorRect.h;
        eVideo::GetScissor(&oldScissor);
        eVideo::SetScissor(&newScissor);

        mVec2 offset;
        offset.x = -*(float *)((char *)this + 0xB4);
        offset.y = -*(float *)((char *)this + 0xB8);
        eSprite_GetScreenPos(&camera, *(int *)((char *)this + 0x4C), offset,
                             pos3, &pos2, &drawArea);
        drawClip.x = drawArea.x;
        drawClip.y = drawArea.y;
        drawClip.w = drawArea.w;
        drawClip.h = drawArea.h;
    }

    float rowPos = pos2.y;
    float *rowHeights = *(float **)((char *)this + 0xC0);
    int row = -1;
    if (rowHeights != 0) {
        row = (*(int *)((char *)rowHeights - 4) & 0x3FFFFFFF) - 1;
    }

    if (row >= 0) {
        int rowOffset = row << 2;
        do {
            float colPos = pos2.x;
            float *rowHeightPtr = *(float **)((char *)this + 0xC0) + row;
            float *colWidths = *(float **)((char *)this + 0xC4);
            int col = 0;
            int colOffset = 0;
            while (col < array_count(colWidths)) {
                gcUICell cell;
                cell.row = (short)row;
                cell.col = (short)col;

                float colWidth = *(float *)((char *)colWidths + colOffset);
                int cols = array_count(*(void **)((char *)this + 0xC4));
                gcUIWidget **heads = *(gcUIWidget ***)((char *)this + 0xBC);
                gcUIWidget *head = heads[cell.row * cols + cell.col];
                if (head != 0) {
                    float rowHeight = *rowHeightPtr;
                    eRect childRect;
                    childRect.x = colPos;
                    childRect.y = rowPos;
                    childRect.w = colWidth;
                    childRect.h = rowHeight;

                    gcUIWidget *child = head;
                    if (forceDraw != 0) {
                        do {
                            DispatchEntry *drawEntry =
                                (DispatchEntry *)(*(char **)((char *)child + 4) +
                                                  0x100);
                            typedef void (*DrawFn)(
                                void *, const eCamera &, const mOCS &,
                                const eRect &, const eRect &, float, float,
                                float, unsigned int);
                            ((DrawFn)drawEntry->fn)(
                                (char *)child + drawEntry->offset, camera, ocs,
                                childRect, drawClip, drawAlpha, drawScale,
                                drawAngle, flags);
                            child = *(gcUIWidget **)((char *)child + 0x68);
                        } while (child != head);
                    } else {
                        int drawAll = 0;
                        char *dialog = *(char **)((char *)this + 0x20);
                        char *mode = *(char **)(dialog + 0x44);
                        if (mode != 0) {
                            int modeFlag = 0;
                            if ((*(unsigned int *)(mode + 0x50) & 1) != 0) {
                                modeFlag = 1;
                            }
                            modeFlag &= 0xFF;
                            if (modeFlag != 0) {
                                drawAll = 1;
                            }
                        }
                        drawAll &= 0xFF;
                        if (drawAll != 0) {
                            do {
                                DispatchEntry *drawEntry =
                                    (DispatchEntry *)(*(char **)((char *)child + 4) +
                                                      0x100);
                                typedef void (*DrawFn)(
                                    void *, const eCamera &, const mOCS &,
                                    const eRect &, const eRect &, float, float,
                                    float, unsigned int);
                                ((DrawFn)drawEntry->fn)(
                                    (char *)child + drawEntry->offset, camera,
                                    ocs, childRect, drawClip, drawAlpha,
                                    drawScale, drawAngle, flags);
                                child = *(gcUIWidget **)((char *)child + 0x68);
                            } while (child != head);
                        } else {
                            do {
                                float right = childRect.x + childRect.w;
                                int visible = 0;
                                if (drawArea.x < right) {
                                    if (drawArea.y < (childRect.y + childRect.h)) {
                                        if (!((drawArea.x + drawArea.w) <
                                              childRect.x)) {
                                            visible = 1;
                                            if ((drawArea.y + drawArea.h) <
                                                childRect.y) {
                                                visible = 0;
                                            }
                                        }
                                    }
                                }
                                if (visible != 0) {
                                    int clipped = 0;
                                    if (drawClip.x < right) {
                                        if (drawClip.y <
                                            (childRect.y + childRect.h)) {
                                            if (!((drawClip.x + drawClip.w) <
                                                  childRect.x)) {
                                                clipped = 1;
                                                if ((drawClip.y + drawClip.h) <
                                                    childRect.y) {
                                                    clipped = 0;
                                                }
                                            }
                                        }
                                    }
                                    if (clipped != 0) {
                                        DispatchEntry *drawEntry =
                                            (DispatchEntry *)(
                                                *(char **)((char *)child + 4) +
                                                0x100);
                                        typedef void (*DrawFn)(
                                            void *, const eCamera &,
                                            const mOCS &, const eRect &,
                                            const eRect &, float, float, float,
                                            unsigned int);
                                        ((DrawFn)drawEntry->fn)(
                                            (char *)child + drawEntry->offset,
                                            camera, ocs, childRect, drawClip,
                                            drawAlpha, drawScale, drawAngle,
                                            flags);
                                    }
                                }
                                child = *(gcUIWidget **)((char *)child + 0x68);
                            } while (child != head);
                        }
                    }
                }

                colPos = colPos + colWidth;
                col += 1;
                colOffset += 4;
                colWidths = *(float **)((char *)this + 0xC4);
            }

            rowPos = rowPos + *rowHeightPtr;
            row -= 1;
            rowOffset -= 4;
        } while (row >= 0);
    }

    if (((*(unsigned int *)((char *)this + 0xB0) & 0x10) != 0) & 0xFF) {
        eVideo::SetScissor(&oldScissor);
    }
}
