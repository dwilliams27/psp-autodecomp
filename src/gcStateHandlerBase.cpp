// gcStateHandlerBase — base class for state-machine handlers.

class cBase;

class gcStateHandlerBase {
public:
    gcStateHandlerBase(cBase *);
};

// ── gcStateHandlerBase::gcStateHandlerBase(cBase *) @ 0x00109D84 ──
gcStateHandlerBase::gcStateHandlerBase(cBase *parent) {
    *(cBase **)((char *)this + 0) = parent;
    *(void **)((char *)this + 4) = (void *)0x388280;
    *(int *)((char *)this + 0x08) = 0;
    *(int *)((char *)this + 0x0C) = 0;
    *(int *)((char *)this + 0x10) = 4;
    *(int *)((char *)this + 0x14) = 0;
    *(int *)((char *)this + 0x18) = 0;
    *(int *)((char *)this + 0x1C) = (int)this | 1;
    *(int *)((char *)this + 0x20) = 0;
    *(int *)((char *)this + 0x24) = 0;
    *(int *)((char *)this + 0x28) = (int)this | 1;
}
