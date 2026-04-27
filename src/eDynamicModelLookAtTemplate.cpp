// Multi-class wrapper file for small leaf GetName/GetText methods.
// Functions:
//   eDynamicModelLookAtTemplate::eDynamicModelLookAtTemplate(cBase *)  @ 0x0004b3a4
//   eDynamicModelLookAtTemplate::GetName(char *) const           @ 0x001f0fdc
//   gcStateHandlerBase::GetName(char *) const                    @ 0x002571a8
//   gcEvent::GetName(char *) const                               @ 0x0023a7e0
//   gcLobbyConfigStrings::GetName(char *) const                  @ 0x002807a8
//   gcValLobbyConnectionStatus::GetText(char *) const            @ 0x00347c9c

class cBase;

void cStrCopy(char *, const char *);
void cStrAppend(char *, const char *, ...);

extern "C" void cHandlePairT_GetName(void *, char *, int, int);

extern const char gcLobbyConfigStrings_fmt[];               // @ 0x36E068
extern const char gcValLobbyConnectionStatus_fmt[];         // @ 0x36F4AC
extern const char gcValGetText_text[];                      // @ 0x36DAF0

extern char eDynamicModelLookAtTemplatevirtualtable[];      // @ 0x3813A8

// ──────────────────────────────────────────────────────────────────────────
// eDynamicModelLookAtTemplate::eDynamicModelLookAtTemplate(cBase *)  @ 0x0004b3a4
// eDynamicModelLookAtTemplate::GetName(char *) const  @ 0x001f0fdc
// ──────────────────────────────────────────────────────────────────────────

class eDynamicModelLookAtTemplate {
public:
    eDynamicModelLookAtTemplate(cBase *);
    void GetName(char *) const;
};

eDynamicModelLookAtTemplate::eDynamicModelLookAtTemplate(cBase *parent) {
    *(cBase **)((char *)this + 0) = parent;
    *(void **)((char *)this + 4) = eDynamicModelLookAtTemplatevirtualtable;
    __asm__ volatile("" ::: "memory");
    *(char *)((char *)this + 8) = 1;
    *(float *)((char *)this + 0xC) = 0.0f;
    *(float *)((char *)this + 0x10) = 0.0f;
    *(float *)((char *)this + 0x14) = 0.0f;
    *(float *)((char *)this + 0x18) = 0.0f;
    *(float *)((char *)this + 0x1C) = 0.0f;
    *(short *)((char *)this + 0x34) = 0;
    *(short *)((char *)this + 0x36) = 0;
    *(char *)((char *)this + 0x20) = 0;
}

void eDynamicModelLookAtTemplate::GetName(char *buf) const {
    cStrCopy(buf, (const char *)((char *)this + 32));
}

#pragma control sched=2

// ──────────────────────────────────────────────────────────────────────────
// gcStateHandlerBase::GetName(char *) const  @ 0x002571a8
// ──────────────────────────────────────────────────────────────────────────

class gcStateHandlerBase {
public:
    void GetName(char *) const;
};

void gcStateHandlerBase::GetName(char *buf) const {
    cHandlePairT_GetName((char *)this + 8, buf, 0, 1);
}

// ──────────────────────────────────────────────────────────────────────────
// gcEvent::GetName(char *) const  @ 0x0023a7e0
// ──────────────────────────────────────────────────────────────────────────

class gcEvent {
public:
    void GetName(char *) const;
};

struct gcEvent_data {
    char _pad[16];
    const char *mName;
};

void gcEvent::GetName(char *buf) const {
    *buf = 0;
    if (((const gcEvent_data *)this)->mName != 0) {
        cStrCopy(buf, ((const gcEvent_data *)this)->mName);
    }
}

// ──────────────────────────────────────────────────────────────────────────
// gcLobbyConfigStrings::GetName(char *) const  @ 0x002807a8
// ──────────────────────────────────────────────────────────────────────────

class gcLobbyConfigStrings {
public:
    void GetName(char *) const;
};

void gcLobbyConfigStrings::GetName(char *buf) const {
    cStrAppend(buf, gcLobbyConfigStrings_fmt, gcValGetText_text);
}

// ──────────────────────────────────────────────────────────────────────────
// gcValLobbyConnectionStatus::GetText(char *) const  @ 0x00347c9c
// ──────────────────────────────────────────────────────────────────────────

class gcValLobbyConnectionStatus {
public:
    void GetText(char *) const;
};

void gcValLobbyConnectionStatus::GetText(char *buf) const {
    cStrAppend(buf, gcValLobbyConnectionStatus_fmt, gcValGetText_text);
}
